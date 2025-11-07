#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"

#include "Inv_InventoryGrid.generated.h"

class UInv_ItemComponent;
struct FInv_ItemManifest;
class UCanvasPanel;
class UInv_GridSlot;
class UInv_InventoryComponent;

/**
 *
 */
UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override; // Viewport를 동시에 생성하는 것이 NativeConstruct?

	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* ItemComponent); // 아이템을 위한 공간이 있는지 확인

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item); // 아이템 추가

private:

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

	void ConstructGrid();
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item); // 인벤토리 항목으로 item이 있는 공간이 있을 수 있어서 만드는 것?
	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& Manifest); // 나중에 Builds 만들 때 사용하는 공간인가?
	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* NewItem);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	EInv_ItemCategory ItemCategory;

	//2차원 격자를 만드는 것 Tarray로
	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")	
	TSubclassOf<UInv_GridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	// 왜 굳이 int32로?
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float TileSize;

	bool MatchesCategory(const UInv_InventoryItem* Item) const;
};


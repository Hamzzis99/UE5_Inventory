#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"

#include "Inv_InventoryGrid.generated.h"

struct FInv_ImageFragment;
struct FInv_GridFragment;
class UInv_SlottedItem;
class UInv_ItemComponent;
struct FInv_ItemManifest;
class UCanvasPanel;
class UInv_GridSlot;
class UInv_InventoryComponent;
struct FGameplayTag;

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
	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* NewItem); // 아이템을 인덱스에 추가
	bool MatchesCategory(const UInv_InventoryItem* Item) const; // 카테고리 일치 여부 확인
	FVector2D GetDrawSize(const FInv_GridFragment* GridFragment) const; // 그리드 조각의 그리기 크기 가져오기
	void SetSlottedItemImage(const UInv_SlottedItem* SlottedItem, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment) const; // 슬로티드 아이템 이미지 설정
	void AddItemAtIndex(UInv_InventoryItem* Item, int32 Index, const bool bStackable, const int32 StackAmount); // 인덱스에 아이템 추가
	UInv_SlottedItem* CreateSlottedItem(UInv_InventoryItem* Item,
		const bool bStackable,
		const int32 StackAmount,
		const FInv_GridFragment* GridFragment,
		const FInv_ImageFragment* ImageFragment,
		const int32 Index
	); // 슬로티드 아이템 생성
	void AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment, UInv_SlottedItem* SlottedItem) const;
	void UpdateGridSlots(UInv_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount); // 그리드 슬롯 업데이트
	bool IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const; // 인덱스가 이미 점유되었는지 확인
	bool HasRoomAtIndex(const UInv_GridSlot* GridSlot,
		const FIntPoint& Dimensions,
		const TSet<int32>& CheckedIndices,
		TSet<int32>& OutTentativelyClaimed,
		const FGameplayTag& ItemType,
		const int32 MaxStackSize);

	bool CheckSlotConstraints(const UInv_GridSlot* GridSlot,
		const UInv_GridSlot* SubGridSlot,
		const TSet<int32>& CheckedIndices,
		TSet<int32>& OutTentativelyClaimed,
		const FGameplayTag& ItemType,
		const int32 MaxStackSize) const;
	FIntPoint GetItemDimensions(const FInv_ItemManifest& Manifest) const; // 아이템 치수 가져오기
	bool HasValidItem(const UInv_GridSlot* GridSlot) const; // 그리드 슬롯에 유효한 아이템이 있는지 확인
	bool IsUpperLeftSlot(const UInv_GridSlot* GridSlot, const UInv_GridSlot* SubGridSlot) const; // 그리드 슬롯이 왼쪽 위 슬롯인지 확인
	bool DoesItemTypeMatch(const UInv_InventoryItem* SubItem, const FGameplayTag& ItemType) const; // 아이템 유형이 일치하는지 확인
	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const; // 그리드 경계 내에 있는지 확인
	int32 DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 AmountToFill, const UInv_GridSlot* GridSlot) const;
	int32 GetStackAmount(const UInv_GridSlot* GridSlot) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	EInv_ItemCategory ItemCategory;

	//2차원 격자를 만드는 것 Tarray로
	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")	
	TSubclassOf<UInv_GridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_SlottedItem> SlottedItemClass; 

	UPROPERTY()
	TMap<int32, TObjectPtr<UInv_SlottedItem>> SlottedItems; // 인덱스와 슬로티드 아이템 매핑 아이템을 등록할 때마다 이 것을 사용할 것.

	// 왜 굳이 int32로?
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float TileSize;
};


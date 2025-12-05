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
class UInv_HoverItem;

/**
 *
 */
UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override; // Viewport를 동시에 생성하는 것이 NativeConstruct?
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // 매 프레임마다 호출되는 틱 함수 (마우스 Hover에 사용)

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
	
	/* 아이템 마우스 클릭 판단*/
	bool IsRightClick(const FPointerEvent& MouseEvent) const;
	bool IsLeftClick(const FPointerEvent& MouseEvent) const;
	void PickUp(UInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex); // 이 픽업은 마우스로 아이템을 잡을 때
	void AssignHoverItem(UInv_InventoryItem* InventoryItem); // 아이템 기반 호버 아이템 할당
	void AssignHoverItem(UInv_InventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex); // 인덱스 기반 호버 아이템 할당
	void RemoveItemFromGrid(UInv_InventoryItem* InventoryItem, const int32 GridIndex); // 그리드에서 아이템 제거
	void UpdateTileParameters(const FVector2D CanvasPosition, const FVector2D MousePosition); // 타일 매개변수 업데이트
	FIntPoint CalculateHoveredCoordinates(const FVector2D CanvasPosition, const FVector2D MousePosition) const; // 호버된 좌표 계산
	EInv_TileQuadrant CalculateTileQuadrant(const FVector2D CanvasPosition, const FVector2D MousePosition) const; // 타일 사분면 계산
	void OnTileParametersUpdate(const FInv_TileParameters& Parameters); // 타일 매개변수 업데이트시 호출되는 함수
	FIntPoint CalculateStartingCoordinate(const FIntPoint& Coordinate, const FIntPoint& Dimensions, const EInv_TileQuadrant Quadrant) const; // 문턱을 얼마나 넘을 수 있는지.
	FInv_SpaceQueryResult CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions) const; // 호버 위치 확인

	UFUNCTION()
	void AddStacks(const FInv_SlotAvailabilityResult& Result);

	UFUNCTION()
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent); // 슬롯 아이템 클릭시 호출되는 함수


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

	//포인터를 생성하기 위한 보조 클래스
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_HoverItem> HoverItemClass;

	UPROPERTY()
	TObjectPtr<UInv_HoverItem> HoverItem;

	//아이템이 프레임마다 매개변수를 어떻게 받을지 계산.
	FInv_TileParameters TileParameters;
	FInv_TileParameters LastTileParameters;

	// Index where an item would be placed if we click on the grid at a valid location
	// 아이템이 유효한 위치에 그리드를 클릭하면 배치될 인덱스
	int32 ItemDropIndex{ INDEX_NONE };
	FInv_SpaceQueryResult CurrentQueryResult; // 현재 쿼리 결과
};


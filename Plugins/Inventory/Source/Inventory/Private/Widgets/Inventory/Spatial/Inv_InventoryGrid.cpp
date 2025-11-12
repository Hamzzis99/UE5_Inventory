// Gihyeon's Inventory Project
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"

void UInv_InventoryGrid::NativeConstruct()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer()); // 플레이어의 인벤토리 컴포넌트를 가져온다.
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem); // 델리게이트 바인딩 
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_ItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_InventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const FInv_ItemManifest& Manifest)
{
	FInv_SlotAvailabilityResult Result;
	Result.TotalRoomToFill = 1;

	// 아이템을 넣어보자!
	FInv_SlotAvailability SlotAvailability;
	SlotAvailability.AmountToFill = 1; // 아이템으로부터 공간 채우기
	SlotAvailability.Index = 0;	// 슬롯 가능 여부 추가

	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability));

	return Result;
}

// 인벤토리 스택 쌓는 부분.
void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item)
{
	//아이템 그리드 체크 부분?
	if (!MatchesCategory(Item)) return;

	//공간이 있다고 부르는 부분.
	FInv_SlotAvailabilityResult Result = HasRoomForItem(Item);

	// Create a widget to show the item icon and add it to the correct spot on the grid.
	AddItemToIndices(Result, Item);

}

void UInv_InventoryGrid::AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* NewItem) 
{
	//격자의 크기를 얻어오자. 게임플레이 태그로 말야
	// Get Grid Fragment so we know how many grid spaces the item takes.
		// 텍스처와 아이콘도 여기서 얻어온다는건가?
	// Get Image Fragment so we have the icon to display
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(NewItem, FragmentTags::GridFragment);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(NewItem, FragmentTags::IconFragment);
	if (!GridFragment || !ImageFragment) return; // 둘 중 하나라도 없으면 리턴




	// Create a widget to add to the grid
	UInv_SlottedItem* SlottedItemWidget = CreateWidget<UInv_SlottedItem>(GetOwningPlayer(), SlottedItemClass);
	SlottedItem->SetInventoryItem(NewItem);

	//슬레이트 브러시?
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon()); // 아이콘 설정
	Brush.DrawAs = ESlateBrushDrawType::Image; // 이미지로 그리기


	// 삭제 소비 파괴 했을 때 이곳에.
	// Store the new widget in a container.

}

//2차원 격자 생성 아이템칸 만든다는 뜻.
void UInv_InventoryGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns); // Tarray 지정 하는 건 알겠는데 GridSlot이거 어디서?

	for (int32 j = 0; j < Rows; ++j)
	{
		for (int32 i = 0; i < Columns; ++i)
		{
			UInv_GridSlot* GridSlot = CreateWidget<UInv_GridSlot>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);

			const FIntPoint TilePosition(i, j);
			GridSlot->SetTileIndex(UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot); // 슬롯 사용한다는 건가?
			GridCPS->SetSize(FVector2D(TileSize, TileSize)); // 사이즈 조정
			GridCPS->SetPosition(TilePosition * TileSize); // 위치 조정
			
			GridSlots.Add(GridSlot);
		}
	}
}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory; // 아이템 카테고리 비교
}

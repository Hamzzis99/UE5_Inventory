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
#include "Items/Fragments/Inv_FragmentTags.h"
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
	Result.bStackable = true;

	// 아이템을 넣어보자! (임시로 2칸 5칸 있다고 가정) 디버깅용도
	FInv_SlotAvailability SlotAvailability;
	SlotAvailability.AmountToFill = 2; // 아이템으로부터 공간 채우기
	SlotAvailability.Index = 0;	// 슬롯 가능 여부 추가
	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability));

	FInv_SlotAvailability SlotAvailability2;
	SlotAvailability2.AmountToFill = 5; // 아이템으로부터 공간 채우기
	SlotAvailability2.Index = 1;	// 슬롯 가능 여부 추가
	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability2));
	

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
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		AddItemAtIndex(NewItem, Availability.Index, Result.bStackable, Availabilty.AmountToFill); // 인덱스에 아이템 추가
		UpdateGridSlots(NewItem, Availability.Index, Result.bStackable, Availabilty.AmountToFill); //그리드 슬롯 업데이트 부분
	}
}

FVector2D UInv_InventoryGrid::GetDrawSize(const FInv_GridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2; // 아이콘 타일 너비 계산

	return GridFragment->GetGridSize() * IconTileWidth; // 아이콘 크기 반환
}

void UInv_InventoryGrid::SetSlottedItemImage(const UInv_SlottedItem* SlottedItem, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment) const
{
	//슬레이트 브러시?
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon()); // 아이콘 설정
	Brush.DrawAs = ESlateBrushDrawType::Image; // 이미지로 그리기
	Brush.ImageSize = GetDrawSize(GridFragment); // 아이콘 크기 설정
	SlottedItem->SetImageBrush(Brush); // 슬로티드 아이템에 브러시 설정
}

void UInv_InventoryGrid::AddItemAtIndex(UInv_InventoryItem* Item, int32 Index, const bool bStackable, const int32 StackAmount)
{
	//격자의 크기를 얻어오자. 게임플레이 태그로 말야
	// Get Grid Fragment so we know how many grid spaces the item takes.
	// 텍스처와 아이콘도 여기서 얻어온다는건가?
	// Get Image Fragment so we have the icon to display
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::GridFragment);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(Item, FragmentTags::IconFragment);
	if (!GridFragment || !ImageFragment) return; // 둘 중 하나라도 없으면 리턴

	// Add the slotted item to the canvas panel.
	// 슬롯 아이템을 캔버스 패널에 그려주는 곳. 또한 그리드 슬롯을 관리해주는 곳.
	UInv_SlottedItem* SlottedItem = CreateSlottedItem(Item, bStackable, StackAmount, GridFragment, ImageFragment, Index);
	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);

	// 삭제 소비 파괴 했을 때 이곳에.
	// Store the new widget in a container.
	SlottedItems.Add(Index, SlottedItem); // 인덱스와 슬로티드 아 이템 매핑
}

UInv_SlottedItem* UInv_InventoryGrid::CreateSlottedItem(UInv_InventoryItem* Item, const bool bStackable, const int32 StackAmount, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment, const int32 Index)
{
	// Create a widget to add to the grid
	UInv_SlottedItem* SlottedItem = CreateWidget<UInv_SlottedItem>(GetOwningPlayer(), SlottedItemClass);
	SlottedItem->SetInventoryItem(Item);
	SetSlottedItemImage(SlottedItem, GridFragment, ImageFragment);
	SlottedItem->SetGridIndex(Index);
	SlottedItem->SetIsStackable(bStackable);
	const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
	SlottedItem->UpdateStackCount(StackUpdateAmount);

	return SlottedItem;
}

void UInv_InventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment, UInv_SlottedItem* SlottedItem) const
{
	CanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasSlot->SetSize(GetDrawSize(GridFragment));
	const FVector2D DrawPos = UInv_WidgetUtils::GetPositionFromIndex(Index, Columns)* TileSize; // 정사각형 위치를 의미하는 것?
	const FVector2D DrawPosWithPadding = DrawPos + FVector2D(GridFragment->GetGridPadding()); // 패딩 적용된 위치
	CanvasSlot->SetPosition(DrawPosWithPadding);
}

void UInv_InventoryGrid::UpdateGridSlots(UInv_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount)
{
	check(GridSlots.IsValidIndex(Index)); // 인덱스 유효성 검사
	
	//쌓을 수 있는 아이템인지 확인해볼까? (Stackable이 가능한지)
	if (bStackableItem)
	{
		GridSlots[Index]->SetInventoryItem(NewItem); // 그리드 슬롯에 인벤토리 아이템 설정
	}

	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(NewItem, FragmentTags::GridFragment); // 그리드 조각 가져오기
	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1,1); // 그리드 크기 가져오기

	//2D 격자 순회하면서 그리드 슬롯 업데이트
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UInv_GridSlot* GridSlot) // [&] 이건 뭔데?
	{
		GridSlot->SetInventoryItem(NewItem); // 그리드 슬롯에 인벤토리 아이템 설정
		GridSlot->SetUpperLeftIndex(Index); // 그리드 슬롯에 왼쪽 위 인덱스 설정
		GridSlot->SetOccupiedTexture(); // 그리드 슬롯을 점유된 텍스처로 설정 (그니까 아이템을 격자칸들 수만큼 공간으로 채운다는 것)
		GridSlot->SetAvailable(false); // 그리드 슬롯을 사용 불가능으로 설정
	}); //람다함수 부분들
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


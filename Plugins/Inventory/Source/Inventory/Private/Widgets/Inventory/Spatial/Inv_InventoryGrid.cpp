// Gihyeon's Inventory Project
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Items/Manifest/Inv_ItemManifest.h"
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
	FInv_SlotAvailabilityResult Result; // GridTypes.h에서 참고해야할 구조체.
	
	// 아이템을 쌓을 수 있는지 판단하기.
	// Determine if the item is stackable.
	const FInv_StackableFragment* StackableFragment = Manifest.GetFragmentOfType<FInv_StackableFragment>();
	Result.bStackable = StackableFragment != nullptr; // nullptr가 아니라면 쌓을 수 있다!

	// 얼마나 쌓을 수 있는지 판단하는 부분 만들기.
	// Determine how many stacks to add.
	const int32 MaxStackSize = StackableFragment ? StackableFragment->GetMaxStackSize() : 1; // 스택 최대 크기 얻기
	int32 AmountToFill = StackableFragment ? StackableFragment->GetMaxStackSize() : 1; // 널포인트가 아니면 스택을 쌓아준다. 다만 이쪽은 변경 가능하게. 채울 양을 업데이트 해야하니.

	TSet<int32> CheckedIndices; // 이미 확인한 인덱스 집합
	//그리드 슬롯을 반복하여서 확인하기.
	// For each Grid Slot:
	for (const auto& GridSlot : GridSlots)
	{
		// ➡️ 더 이상 채울 아이템이 없다면, (루프를) 일찍 빠져나옵니다.
		// If we don't have anymore to fill, break out of the early
		if (AmountToFill == 0) break;

		// 이 인덱스가 이미 점유되어있는지 확인하기
		// Is this Index claimed yet?
		if (IsIndexClaimed(CheckedIndices, GridSlot->GetIndex())) continue; // 이미 점유되어 있다면 다음으로 넘어간다. bool값으로 확인.

		// Is the item in grid bounds?
		if (!IsInGridBounds(GridSlot->GetIndex(), GetItemDimensions(Manifest))) continue; // 그리드 경계 내에 있는지 확인 (넣어도 되거나 안 되는 항목 체크 부분)

		// ➡️ 아이템이 여기에 들어갈 수 있습니까? (예: 그리드 경계를 벗어나지 않는지?)
		// Can the item fit here? (i.e. is it out of grid bounds?)
		TSet<int32> TentativelyClaimed;
		if (!HasRoomAtIndex(GridSlot, GetItemDimensions(Manifest), CheckedIndices, TentativelyClaimed, Manifest.GetItemType(), MaxStackSize))
		{
			continue;// 공간이 없다면 다음으로 넘어간다.
		}

		CheckedIndices.Append(TentativelyClaimed); // 확인된 인덱스에 임시로 점유된 인덱스 추가
		
		
		// 얼마나 채워야해?
		// How much to fill?
		// ➡️ [!] 채워야 할 남은 양을 업데이트합니다.
		// Update the amount left to fill.
	}

	// 모두 반복한 후 나머지는 얼마나 있나요?
	// How much is the Remainder?

	return Result;
}

//2차원 범위 내 각 정사각형의 슬롯 제약 조건을 검사하는 부분들.
//게임 플레이 태그로 구분할 것이다.
bool UInv_InventoryGrid::HasRoomAtIndex(const UInv_GridSlot* GridSlot,
										const FIntPoint& Dimensions,
										const TSet<int32>& CheckedIndices,
										TSet<int32>& OutTentativelyClaimed,
										const FGameplayTag& ItemType,
										const int32 MaxStackSize)
{
	// ➡️ 이 인덱스에 공간이 있습니까? (예: 다른 아이템이 길을 막고 있지 않은지?)
	// Is there room at this index? (i.e are there other items in the way?)
	bool bHasRoomAtIndex = true;
	UInv_InventoryStatics::ForEach2D(GridSlots, GridSlot->GetIndex(), Dimensions, Columns, [&](const UInv_GridSlot* SubGridSlot) 
	{	
		if (CheckSlotConstraints(GridSlot, SubGridSlot, CheckedIndices, OutTentativelyClaimed, ItemType, MaxStackSize))
		{
			OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
		}
		else
		{
			bHasRoomAtIndex = false;
		}
	});

	return bHasRoomAtIndex; 
}

//이 제약조건을 다 확인해야 인벤토리에 공간이 있는지 확인해주는 것이다.
bool UInv_InventoryGrid::CheckSlotConstraints(const UInv_GridSlot* GridSlot,
	const UInv_GridSlot* SubGridSlot,
	const TSet<int32>& CheckedIndices,
	TSet<int32>& OutTentativelyClaimed,
	const FGameplayTag& ItemType,
	const int32 MaxStackSize) const
{		
	// Index claimed? 
	// 점유되어 있는지 확인한다.
	if (IsIndexClaimed(CheckedIndices, SubGridSlot->GetIndex())) return false; // 인덱스가 이미 사용하면 false를 반환하는 부분.

	// 유효한 항목이 있습니까?
	// Has valid item? 
	if (!HasValidItem(SubGridSlot))
	{
		OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
		return true;
	}

	// 이 격자가 왼쪽으로 가는 것이 맞을까요?
	// Is this Grid Slot an upper left slot?
	if (!IsUpperLeftSlot(GridSlot, SubGridSlot)) return false; // 왼쪽 위 슬롯이 아니라면 false 반환.

	// ➡️ [!] (항목이 있다면) 스택 가능한 아이템입니까?
	// If so, it this a stackable item?
	const UInv_InventoryItem* SubItem = SubGridSlot->GetInventoryItem().Get();

	// 이 아이템이 우리가 추가하려는 아이템과 동일한 유형인가?
	// Is this item the same type as item we're trying to add?
	if (!DoesItemTypeMatch(SubItem, ItemType)) return false;

	// ➡️ [!] 스택 가능하다면, 이 슬롯은 이미 최대 스택 크기입니까?
	// If Stackable, is this slot at the max stack size already?
	if (GridSlot->GetStackCount() >= MaxStackSize) return false;
	 
	return true;
}

FIntPoint UInv_InventoryGrid::GetItemDimensions(const FInv_ItemManifest& Manifest) const
{
	const FInv_GridFragment* GridFragment = Manifest.GetFragmentOfType<FInv_GridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1); 
}

bool UInv_InventoryGrid::HasValidItem(const UInv_GridSlot* GridSlot) const
{
	return GridSlot->GetInventoryItem().IsValid();
}

bool UInv_InventoryGrid::IsUpperLeftSlot(const UInv_GridSlot* GridSlot, const UInv_GridSlot* SubGridSlot) const
{
	return SubGridSlot->GetUpperLeftIndex() == GridSlot->GetIndex();
}

bool UInv_InventoryGrid::DoesItemTypeMatch(const UInv_InventoryItem* SubItem, const FGameplayTag& ItemType) const
{
	return SubItem->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
}

bool UInv_InventoryGrid::IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const
{
	if (StartIndex < 0 || StartIndex >= GridSlots.Num()) return false;
	const int32 EndColumn = (StartIndex % Columns) + ItemDimensions.X;
	const int32 EndRow = (StartIndex / Columns) + ItemDimensions.Y;
	return EndColumn <= Columns && EndRow <= Rows;
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
		AddItemAtIndex(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);// 인덱스에 아이템 추가
		UpdateGridSlots(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);//그리드 슬롯 업데이트 부분
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

bool UInv_InventoryGrid::IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const
{
	return CheckedIndices.Contains(Index);
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


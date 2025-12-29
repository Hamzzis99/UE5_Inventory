// Gihyeon's Inventory Project


#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"

#include "Inventory.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Inventory.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Items/Inv_InventoryItem.h"
#include "Widgets/ItemDescription/Inv_ItemDescription.h"
#include "Blueprint/WidgetTree.h"
#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Widgets/Inventory/SlottedItems/Inv_EquippedSlottedItem.h"

//버튼 생성할 때 필요한 것들
void UInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//인벤토리 장비 칸들
	Button_Equippables->OnClicked.AddDynamic(this, &ThisClass::ShowEquippables);
	Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this, &ThisClass::ShowCraftables);
	
	// 툴팁 캔버스 설정
	Grid_Equippables->SetOwningCanvas(CanvasPanel);
	Grid_Consumables->SetOwningCanvas(CanvasPanel);
	Grid_Craftables->SetOwningCanvas(CanvasPanel);

	ShowEquippables(); // 기본값으로 장비창을 보여주자.
	
	WidgetTree->ForEachWidget([this](UWidget* Widget) // 위젯 트리의 각 위젯에 대해 반복
	{
		UInv_EquippedGridSlot* EquippedGridSlot = Cast<UInv_EquippedGridSlot>(Widget); // 장착된 그리드 슬롯으로 캐스팅
		if (IsValid(EquippedGridSlot))
		{
			EquippedGridSlots.Add(EquippedGridSlot); // 장착된 그리드 슬롯을 배열에 추가
			EquippedGridSlot->EquippedGridSlotClicked.AddDynamic(this, &ThisClass::EquippedGridSlotClicked); // 클릭 이벤트 바인딩
		}
	});
}

// 장착된 그리드 슬롯이 클릭되었을 때 호출되는 함수
void UInv_SpatialInventory::EquippedGridSlotClicked(UInv_EquippedGridSlot* EquippedGridSlot,const FGameplayTag& EquipmentTypeTag) // 콜백함수 
{
	// Check to see if we can equip the Hover Item
	// 호버 아이템을 장착할 수 있는지 확인
	if (!CanEquipHoverItem(EquippedGridSlot, EquipmentTypeTag)) return; // 장착할 수 없으면 반환
	
	UInv_HoverItem* HoverItem = GetHoverItem();
	
	// Create an Equipped Slotted Item and add it to the Equipped Grid Slot (call EquippedGridSlot->OnItemEquipped())
	// 장착된 슬롯 아이템을 만들고 장착된 그리드 슬롯에 (EquippedGridSlot->OnItemEquipped()) 추가
	const float TileSize = UInv_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetTileSize();
	
	// 장착시킨 그리드 슬롯에 실제 아이템 장착
	UInv_EquippedSlottedItem* EquippedSlottedItem = EquippedGridSlot->OnItemEquipped(
		HoverItem->GetInventoryItem(),
		EquipmentTypeTag,
		TileSize
	);
	EquippedSlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	// Clear the Hover item
	// 호버 아이템 지우기
	
	// Inform the server that we've equipped an item (potentially unequipping an item as well)
	// 아이템을 장착했음을 서버에 알리기(잠재적으로 아이템을 해제하기도 함)
	
}

// 장착된 슬롯 아이템 클릭 시 호출되는 함수
void UInv_SpatialInventory::EquippedSlottedItemClicked(UInv_EquippedSlottedItem* SlottedItem)
{
	
}

// 마우스 버튼 다운 이벤트 처리 인벤토리 아이템 드롭
FReply UInv_SpatialInventory::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	ActiveGrid->DropItem();
	return FReply::Handled();
}

// 매 프레임마다 호출되는 틱 함수 (마우스 Hover에 사용)
void UInv_SpatialInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!IsValid(ItemDescription)) return; // 아이템 설명 위젯이 유효하지 않으면 반환
	SetItemDescriptionSizeAndPosition(ItemDescription, CanvasPanel); // 아이템 설명 크기 및 위치 설정
}

// 마우스를 올려둘 때 뜨는 아이템 설명 크기 및 위치 설정
void UInv_SpatialInventory::SetItemDescriptionSizeAndPosition(UInv_ItemDescription* Description, UCanvasPanel* Canvas) const
{
	UCanvasPanelSlot* ItemDescriptionCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Description);
	if (!IsValid(ItemDescriptionCPS)) return;

	const FVector2D ItemDescriptionSize = Description->GetBoxSize();
	ItemDescriptionCPS->SetSize(ItemDescriptionSize);

	FVector2D ClampedPosition = UInv_WidgetUtils::GetClampedWidgetPosition(
		UInv_WidgetUtils::GetWidgetSize(Canvas),
		ItemDescriptionSize,
		UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));

	ItemDescriptionCPS->SetPosition(ClampedPosition);
}

// 호버 아이템 장착 가능 여부 확인 게임태그도 참조해야 낄 수 있게.
bool UInv_SpatialInventory::CanEquipHoverItem(UInv_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag) const 
{
	if (!IsValid(EquippedGridSlot) || !EquippedGridSlot->GetInventoryItem().IsValid()) return false; // 장착된 그리드 슬롯이 유효하지 않거나 인벤토리 아이템이 유효하지 않으면 false 반환

	UInv_HoverItem* HoverItem = GetHoverItem();
	if (!IsValid(HoverItem)) return false; // 호버 아이템이 유효하지 않으면 false 반환
	
	UInv_InventoryItem* HeldItem = HoverItem->GetInventoryItem(); // 호버 아이템에서 인벤토리 아이템 가져오기
	
	// Check if the held item is non-stackable and equippable
	// 들고 있는 아이템이 스택 불가능하고 장착 가능한지 확인
	return HasHoverItem() && 
		IsValid(HeldItem) && 
			!HoverItem->IsStackable() && 
				HeldItem->GetItemManifest().GetItemCategory() == EInv_ItemCategory::Equippable &&
					HeldItem->GetItemManifest().GetItemType().MatchesTag(EquipmentTypeTag);
}


FInv_SlotAvailabilityResult UInv_SpatialInventory::HasRoomForItem(UInv_ItemComponent* ItemComponent) const // 아이템 컴포넌트가 있는지 확인
{
	switch (UInv_InventoryStatics::GetItemCategoryFromItemComp(ItemComponent))
	{
	case EInv_ItemCategory::Equippable:
		return Grid_Equippables->HasRoomForItem(ItemComponent);
	case EInv_ItemCategory::Consumable:
		return Grid_Consumables->HasRoomForItem(ItemComponent);
	case EInv_ItemCategory::Craftable:
		return Grid_Craftables->HasRoomForItem(ItemComponent);
	default:
		UE_LOG(LogInventory, Error, TEXT("ItemComponent doesn't have a valid Item Category. (inventory.h)"))
			return FInv_SlotAvailabilityResult(); // 빈 결과 반환
	}
}

// 아이템이 호버되었을 때 호출되는 함수 (설명 칸 보일 때 쓰는 부분들임)
void UInv_SpatialInventory::OnItemHovered(UInv_InventoryItem* Item)
{
	const auto& Manifest = Item->GetItemManifest(); // 아이템 매니페스트 가져오기
	UInv_ItemDescription* DescriptionWidget = GetItemDescription();
	DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer); // 기존 타이머 클리어
	
	FTimerDelegate DescriptionTimerDelegate;
	DescriptionTimerDelegate.BindLambda([this, &Manifest, DescriptionWidget]()
	{
		// Assimalate the manifest into the Item Description widget.
		// 아이템 설명 위젯에 매니페스트 동화
		Manifest.AssimilateInventoryFragments(DescriptionWidget);
		GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible); // 설명 위젯 보이기
	});
	
	// 타이머 설정
	GetOwningPlayer()->GetWorldTimerManager().SetTimer(DescriptionTimer, DescriptionTimerDelegate, DescriptionTimerDelay, false);
}

//아이템에서 마우스에 손을 땔 떄
void UInv_SpatialInventory::OnItemUnHovered()
{
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed); // 설명 위젯 숨기기
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer); // 타이머 클리어
}

bool UInv_SpatialInventory::HasHoverItem() const // UI 마우스 호버 부분들
{
	if (Grid_Equippables->HasHoverItem()) return true;
	if (Grid_Consumables->HasHoverItem()) return true;
	if (Grid_Craftables->HasHoverItem()) return true;
	return false;
}

// 활성 그리드가 유효한 경우 호버 아이템 반환.
UInv_HoverItem* UInv_SpatialInventory::GetHoverItem() const
{
	if (!ActiveGrid.IsValid()) return nullptr; // 액터 그리드가 유효하지 않으면 nullptr 반환
	
	return ActiveGrid->GetHoverItem(); // 활성 그리드에서 호버 아이템 반환
}

float UInv_SpatialInventory::GetTileSize() const
{
	return Grid_Equippables->GetTileSize(); // 장비 그리드의 타일 크기 반환
}

UInv_ItemDescription* UInv_SpatialInventory::GetItemDescription() // 아이템 설명 위젯 가져오기
{
	if (!IsValid(ItemDescription))
	{
		ItemDescription = CreateWidget<UInv_ItemDescription>(GetOwningPlayer(), ItemDescriptionClass); // 아이템 설명 위젯 생성
		CanvasPanel->AddChild(ItemDescription);
	}
	return ItemDescription;
}

void UInv_SpatialInventory::ShowEquippables()
{
	SetActiveGrid(Grid_Equippables, Button_Equippables);
}

void UInv_SpatialInventory::ShowConsumables()
{
	SetActiveGrid(Grid_Consumables, Button_Consumables);
}

void UInv_SpatialInventory::ShowCraftables()
{
	SetActiveGrid(Grid_Craftables, Button_Craftables);
}



//리펙토링을 이렇게 하네 신기하다. 일단 버튼 비활성화 부분
void UInv_SpatialInventory::DisableButton(UButton* Button)
{
	Button_Equippables->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Craftables->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}

// 그리드가 활성 되면 등장하는 것들.
void UInv_SpatialInventory::SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button) 
{
	if (ActiveGrid.IsValid()) ActiveGrid->HideCursor();
	ActiveGrid = Grid;
	if (ActiveGrid.IsValid()) ActiveGrid->ShowCursor();
	DisableButton(Button);
	Switcher->SetActiveWidget(Grid);
}

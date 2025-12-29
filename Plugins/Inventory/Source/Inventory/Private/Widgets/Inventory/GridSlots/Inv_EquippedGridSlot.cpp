// Gihyeon's Inventory Project


#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"

#include "Components/Image.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"

void UInv_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!IsAvailable()) return;
	UInv_HoverItem* HoverItem = UInv_InventoryStatics::GetHoverItem(GetOwningPlayer());
	if (!IsValid(HoverItem)) return;
	
	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		SetOccupiedTexture(); // 슬롯이 사용 가능한 상태일 때만 텍스처 변경
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInv_EquippedGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (!IsAvailable()) return;
	UInv_HoverItem* HoverItem = UInv_InventoryStatics::GetHoverItem(GetOwningPlayer());
	if (!IsValid(HoverItem)) return;
	
	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		SetUnoccupiedTexture(); // 슬롯이 사용 가능한 상태일 때만 텍스처 변경
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Visible);
	}
}

FReply UInv_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	EquippedGridSlotClicked.Broadcast(this, EquipmentTypeTag); // 델리게이트 브로드캐스트
	return FReply::Handled();
}

// 아이템 장착 시 호출되는 함수
UInv_EquippedSlottedItem* UInv_EquippedGridSlot::OnItemEquipped(UInv_InventoryItem* Item, const FGameplayTag& EquipmentTag, float TileSize)
{
	// Check the Equipment Type Tag
	// 장비 유형 태그 확인
	if (!EquipmentTag.MatchesTagExact(EquipmentTypeTag)) return nullptr;
	
	// Get Grid Dimensions
	// 그리드 크기 가져오기
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::GridFragment);
	if (!GridFragment) return nullptr;
	
	// Calculate the Draw Size for the Equipped Slotted Item
	// 장착된 슬롯 아이템의 그리기 크기 계산
	
	const FIntPoint GridDimensions = GridFragment->GetGridSize();
	
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	const FVector2D DrawSize = GridDimensions * IconTileWidth;
	
	// Create the Equipped Slotted Item Widget
	// 장착된 슬롯 아이템 위젯 생성
	
	// Set the Slotted Item's Inventory Item
	// 슬롯 아이템의 인벤토리 아이템 설정
	
	// Set the Slotted Item's Equipment Type Tag
	// 슬롯 아이템의 장비 유형 태그 설정
	
	// Hide the Stack Count widget on the Slotted Item
	// 슬롯 아이템에서 스택 수량 위젯 숨기기
	
	// Set Inventory Item on this class (the Equipped Grid Slot)
	// 이 클래스(장착된 그리드 슬롯)에 인벤토리 아이템 설정
	
	// Set the Image Brush on the Equipped Slotted Item
	// 장착된 슬롯 아이템에 이미지 브러시 설정
	
	// Add the Slotted Item as a child to this widget's Overlay
	// 이 위젯의 오버레이에 슬롯 아이템을 자식으로 추가
	
	// Return the Equipped Slotted Item
	// 장착된 슬롯 아이템 반환
	return nullptr;
}

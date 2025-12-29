// Gihyeon's Inventory Project


#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"

#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"

void UInv_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!IsAvailable()) return;
	UInv_HoverItem* HoverItem = UInv_InventoryStatics::GetHoverItem(GetOwningPlayer());
	if (!IsValid(HoverItem)) return;
	
	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		SetOccupiedTexture(); // 슬롯이 사용 가능한 상태일 때만 텍스처 변경
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
	}
}

FReply UInv_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	EquippedGridSlotClicked.Broadcast(this, EquipmentTypeTag); // 델리게이트 브로드캐스트
	return FReply::Handled();
}

// Gihyeon's Inventory Project

#pragma once

#include "CoreMinimal.h"
#include "Inv_GridSlot.h"
#include "GameplayTagContainer.h"
#include "Inv_EquippedGridSlot.generated.h"

// 델리게이트 선언: 장착된 그리드 슬롯이 클릭되었을 때 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquippedGridSlotClicked, UInv_EquippedGridSlot*, GridSlot, const FGameplayTag&, EquipmentTypeTag);

class UImage;

UCLASS()
class INVENTORY_API UInv_EquippedGridSlot : public UInv_GridSlot
{
	GENERATED_BODY()
public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	FEquippedGridSlotClicked EquippedGridSlotClicked; // 장착된 그리드 슬롯 클릭 이벤트 델리게이트

private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories = "GameItem.Equipment"))
	FGameplayTag EquipmentTypeTag; // 장착된 아이템의 타입을 나타내는 게임플레이 태그

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GrayedOutIcon; // 호버했을 때 상황
};
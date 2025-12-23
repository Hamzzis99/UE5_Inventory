// Gihyeon's Inventory Project

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Inv_SpatialInventory.generated.h"

class UInv_InventoryGrid;
class UWidgetSwitcher;
class UButton;

// UI 연동 부분들

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_SpatialInventory : public UInv_InventoryBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override; // NativeOnInitialized 이게 뭐지?

	//실제 공간이 있는지 확인하는 곳.
	virtual FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComponent) const override;
	
private: // 여기 있는 UPROPERTY와 위젯과의 이름이 동일해야만함.
	//스위치
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	//메뉴 등록
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Equippables;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Consumables;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Craftables;


	//버튼 등록
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Equippables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Craftables;
	
	UFUNCTION()
	void ShowEquippables();

	UFUNCTION()
	void ShowConsumables();

	UFUNCTION()
	void ShowCraftables();
	

	void DisableButton(UButton* Button);
	void SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button);
	TWeakObjectPtr<UInv_InventoryGrid> ActiveGrid; // 활성 그리드가 생기면 늘 활성해주느 ㄴ함수.
};

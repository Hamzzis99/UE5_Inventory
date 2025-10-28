// Gihyeon's Inventory Project

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Inv_SpatialInventory.generated.h"

class UInv_InventoryGrid;
class UWidgetSwitcher;
class UButton;

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_SpatialInventory : public UInv_InventoryBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override; // NativeOnInitialized �̰� ����?

	//���� ������ �ִ��� Ȯ���ϴ� ��.
	virtual FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComponent) const override;
	
private: // ���� �ִ� UPROPERTY�� �������� �̸��� �����ؾ߸���.
	//����ġ
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	//�޴� ���
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Equippables;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Consumables;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Craftables;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Builds;


	//��ư ���
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Equippables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Craftables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Builds;

	UFUNCTION()
	void ShowEquippables();

	UFUNCTION()
	void ShowConsumables();

	UFUNCTION()
	void ShowCraftables();

	UFUNCTION()
	void ShowBuilds();

	void DisableButton(UButton* Button);

	void SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button);

};

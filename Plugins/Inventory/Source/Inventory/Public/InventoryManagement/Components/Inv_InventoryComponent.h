// Gihyeon's Inventory Project

//�κ��丮 ���̽� �ڽ� Ŭ����

#pragma once

#include "CoreMinimal.h"
//#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Components/ActorComponent.h"
#include "Inv_InventoryComponent.generated.h"

class UInv_InventoryBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable ) // Blueprintable : �������Ʈ���� ���
class INVENTORY_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInv_InventoryComponent();

	void ToggleInventoryMenu(); //�κ��丮 �޴� ��� �Լ�

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	TWeakObjectPtr<APlayerController> OwningController;

	void ConstructInventory();

	UPROPERTY() // �̰Ŵ� �������� Ȯ���ϴ� ��. �������� ���� �ʰ� ���ִ� ��.
	TObjectPtr<UInv_InventoryBase> InventoryMenu; //�κ��丮 �޴� ���� �ν��Ͻ�

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_InventoryBase> InventoryMenuClass;

	bool bInventoryMenuOpen; //�κ��丮 �޴� ���� ����
	void OpenInventoryMenu();
	void CloseInventoryMenu();
};

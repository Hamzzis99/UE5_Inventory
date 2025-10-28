// Gihyeon's Inventory Project

//�κ��丮 ���̽� �ڽ� Ŭ����

#pragma once

#include "CoreMinimal.h"
//#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Components/ActorComponent.h"
#pragma once

#include "Inv_InventoryComponent.generated.h"

class UInv_InventoryItem;
class UInv_InventoryBase;

//��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable ) // Blueprintable : �������Ʈ���� ���
class INVENTORY_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInv_InventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void TryAddItem(UInv_ItemComponent* ItemComponent);

	//���� �κ� RPC�� ���� ��
	UFUNCTION(Server, Reliable) // �ŷ��ϴ� ��? ������ �����ϴ� ��?
	void Server_AddNewItem(UInv_ItemComponent* ItemComponent, int32 StackCount);

	UFUNCTION(Server, Reliable) // �ŷ��ϴ� ��? ������ �����ϴ� ��?
	void Server_AddStacksToItem(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	void ToggleInventoryMenu(); //�κ��丮 �޴� ��� �Լ�

	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
	FNoRoomInInventory NoRoomInInventory;

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

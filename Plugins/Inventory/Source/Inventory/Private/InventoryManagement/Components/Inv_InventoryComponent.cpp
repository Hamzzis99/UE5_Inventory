// Gihyeon's Inventory Project


#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"

// Sets default values for this component's properties
UInv_InventoryComponent::UInv_InventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();
	
}


void UInv_InventoryComponent::ConstructInventory()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Inventory Component should have a Player Controller as Owner."))
	if (!OwningController->IsLocalController()) return;

	//��������� ���� Ŭ������ �����Ǿ� �ִ��� Ȯ��
	InventoryMenu = CreateWidget<UInv_InventoryBase>(OwningController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();

}


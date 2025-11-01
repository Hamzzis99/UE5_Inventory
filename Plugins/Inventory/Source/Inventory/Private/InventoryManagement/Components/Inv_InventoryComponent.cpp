// Gihyeon's Inventory Project


#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"

#include "Net/UnrealNetwork.h"

UInv_InventoryComponent::UInv_InventoryComponent() : InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true); // �⺻������ ���� ����
	bReplicateUsingRegisteredSubObjectList = true; // ��ϵ� ���� ��ü ����� ����Ͽ� ���� ����
	bInventoryMenuOpen = false;	// �κ��丮 �޴��� �����ִ��� ���� �ʱ�ȭ
}

void UInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // ���� �Ӽ� ���� �Լ�
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList); // �κ��丮 ��� ���� ����
}

void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent)
{
	FInv_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);

	if (Result.TotalRoomToFill == 0)
	{
		NoRoomInInventory.Broadcast(); // �� �κ��丮 ��á��! �̰� �˷��ִ°ž�! ��� �߿�߿� ��� �˾Ƶֶ�!
		
	}
	// TODO : ������ �κ��丮�� �߰��ϴ� �κ��� ���� ��. (�ϴ� ���߿�)

	// ������ ���� ���� ������ �����ϴ� ��? ���� RPC�� �غ���.
	if (Result.Item.IsValid() && Result.bStackable) // ��ȿ���� �˻��ϴ� �۾�.
	{
		// �̹� �����ϴ� �����ۿ� ������ �߰��ϴ� �κ�. 
		// Add stacks to an item that already exists in the inventory. We only want to update the stack count,
		// not create a new item of this type.
		Server_AddStacksToItem(ItemComponent, Result.TotalRoomToFill, Result.Remainder);
	}
	// �������� ������ ��� ���.... ������
	else if (Result.TotalRoomToFill > 0)
	{
		// This item type dosen't exist in the inventory. Create a new one and update all partient slots.
		Server_AddNewItem(ItemComponent, Result.bStackable ?  Result.TotalRoomToFill : 0); //���� �� �ִٸ� ä�� �� �ִ� ���� �̷� ������ �� ó�� ����
	}
}

void UInv_InventoryComponent::Server_AddNewItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount) // �������� ���ο� ������ �߰� ����
{
	UInv_InventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);

	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone) // �� �κ��� ������ Ŭ���̾�Ʈ�� ���� ������ �迭 ���� �� �Ǵ� �� (���� ������ ������ �� �����ض�)
	{
		OnItemAdded.Broadcast(NewItem);
	}

	// �������� �����ڸ� ���ִ� ��ǥ�� �δ� ��.

}

void UInv_InventoryComponent::Server_AddStacksToItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{

}

void UInv_InventoryComponent::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
}

void UInv_InventoryComponent::AddRepSubObj(UObject* SubObj)

{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj)) // ���� �غ� �Ǿ����� Ȯ��
	{
		AddReplicatedSubObject(SubObj); // ������ ���� ��ü �߰�
	}
	AddReplicatedSubObject(SubObj);
}


// Called when the game starts
void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();
	
}


//�κ��丮 �޴� ���� ���� �Լ�
void UInv_InventoryComponent::ConstructInventory()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Inventory Component should have a Player Controller as Owner."))
	if (!OwningController->IsLocalController()) return;

	//��������� ���� Ŭ������ �����Ǿ� �ִ��� Ȯ��
	InventoryMenu = CreateWidget<UInv_InventoryBase>(OwningController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
	CloseInventoryMenu();
}

//�κ��丮 �޴� ����/�ݱ� �Լ�
void UInv_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;
	
	if (!OwningController->IsValidLowLevel()) return;

	FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
}

void UInv_InventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if (!OwningController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);
}


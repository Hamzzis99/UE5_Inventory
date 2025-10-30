// Gihyeon's Inventory Project


#include "Items/Components/Inv_ItemComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInv_ItemComponent::UInv_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


	PickupMessage = FString("E - ������ �ݱ�!"); // �⺻ �Ⱦ� �޽��� ����
}

void UInv_ItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, ItemManifest); // ������ �Ŵ��佺Ʈ ���� ����
}

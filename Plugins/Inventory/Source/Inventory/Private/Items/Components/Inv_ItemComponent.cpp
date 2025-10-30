// Gihyeon's Inventory Project


#include "Items/Components/Inv_ItemComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInv_ItemComponent::UInv_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


	PickupMessage = FString("E - Pick Up Item"); // 기본 픽업 메시지 설정 이거 한글로 되는지 한 번 나중에 검사 TEXT를 쓰라는데..
}

void UInv_ItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, ItemManifest); // 아이템 매니페스트 복제 설정
}

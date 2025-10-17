#include "Player/Inv_PlayerController.h"
#include "Inventory.h"

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogInventory, Log, TEXT("BeginPlay for PlayerController")) //임시 로그 사용 부분
}

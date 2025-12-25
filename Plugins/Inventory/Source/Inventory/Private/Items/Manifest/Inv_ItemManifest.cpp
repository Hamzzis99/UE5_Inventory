#include "Items/Manifest/Inv_ItemManifest.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"

UInv_InventoryItem* FInv_ItemManifest::Manifest(UObject* NewOuter) // 인벤토리의 인터페이스? 복사본이라고?
{
	UInv_InventoryItem* Item = NewObject<UInv_InventoryItem>(NewOuter, UInv_InventoryItem::StaticClass()); // 새로운 객체는 뭐가 될지 Input 파라미터

	//재고 항목
	Item->SetItemManifest(*this); // 이 매니페스트로 아이템 매니페스트 설정

	return Item;
}


// 아이템 픽업 액터 생성
void FInv_ItemManifest::SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation) 
{
	// TODO : 아이템 픽업 액터 생성 로직 구현
	if (!(PickupActorClass) || !IsValid(WorldContextObject)) return; // 픽업 액터 클래스가 유효하지 않거나 월드 컨텍스트 객체가 유효하지 않으면 반환
	
	AActor* SpawnActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(PickupActorClass, SpawnLocation, SpawnRotation); // 픽업 액터 생성
	if (!IsValid(SpawnActor)) return;
	
	// Set the item manifest, item category, item type, etc.
	// 아이템 매니페스트, 아이템 카테고리, 아이템 타입 등을 설정하는 부분
	UInv_ItemComponent* ItemComp = SpawnActor->FindComponentByClass<UInv_ItemComponent>();
	check(ItemComp); // ItemComp가 유효한지 확인
	
	ItemComp->InitItemManifest(*this); // 아이템 매니페스트 초기화
}
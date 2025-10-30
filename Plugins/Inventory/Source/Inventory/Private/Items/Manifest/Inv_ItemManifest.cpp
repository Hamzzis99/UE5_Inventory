#include "Items/Manifest/Inv_ItemManifest.h"

UInv_InventoryItem* FInv_ItemManifest::Manifest(UObject* NewOuter) // 인벤토리의 인터페이스? 복사본이라고?
{
	UInv_InventoryItem* Item = NewObject<UInv_InventoryItem>(NewOuter, UInv_InventoryItem::StaticClass()); // 새로운 객체는 뭐가 될지 Input 파라미터

	//재고 항목
	Item->SetItemManifest(*this); /

	return Item;
}
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Items/Inv_InventoryItem.h"

UInv_InventoryItem* FInv_ItemManifest::Manifest(UObject* NewOuter) // �κ��丮�� �������̽�? ���纻�̶��?
{
	UInv_InventoryItem* Item = NewObject<UInv_InventoryItem>(NewOuter, UInv_InventoryItem::StaticClass()); // ���ο� ��ü�� ���� ���� Input �Ķ����

	//��� �׸�
	Item->SetItemManifest(*this); // �� �Ŵ��佺Ʈ�� ������ �Ŵ��佺Ʈ ����

	return Item;
}


// Gihyeon's Inventory Project

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryItem : public UObject
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // �̰� �������ִ� ��
	virtual bool IsSupportedForNetworking() const override { return true; } // ��Ʈ��ŷ ���� ����

	void SetItemManifest(const FInv_ItemManifest& Manifest); // ������ �Ŵ��佺Ʈ ����
	const FInv_ItemManifest& GetItemManifest() const { return ItemManifest.Get<FInv_ItemManifest>();  }
	FInv_ItemManifest& GetItemManifestMutable() { return ItemManifest.GetMutable<FInv_ItemManifest>(); }


private: //�� �κ� GPT �������� �׸� �����ؼ� ������ �����ϴ� ��. �װ� �ٷ� Replicated
	UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/Inventory.Inv_ItemManifest"), Replicated) //�κ��丮 ������ �������Ʈ ����� ��? �Ļ�?
	FInstancedStruct ItemManifest; // instance struct? �̰� ����?
};

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
	
	void SetItemManifest(const FInv_ItemManifest& Manifest); // ������ �Ŵ��佺Ʈ ����

private: //�� �κ� GPT �������� �׸� �����ؼ� ������ �����ϴ� ��. �װ� �ٷ� Replicated
	UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/Inventory.Inv_ItemManifest"), Replicated) //�κ��丮 ������ �������Ʈ ����� ��? �Ļ�?
	FInstancedStruct ItemManifest; // instance struct? �̰� ����?
};

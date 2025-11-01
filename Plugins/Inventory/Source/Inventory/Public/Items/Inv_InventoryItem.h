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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // 이게 복제해주는 것
	virtual bool IsSupportedForNetworking() const override { return true; } // 네트워킹 지원 여부

	void SetItemManifest(const FInv_ItemManifest& Manifest); // 아이템 매니페스트 설정
	const FInv_ItemManifest& GetItemManifest() const { return ItemManifest.Get<FInv_ItemManifest>();  }
	FInv_ItemManifest& GetItemManifestMutable() { return ItemManifest.GetMutable<FInv_ItemManifest>(); }


private: //이 부분 GPT 돌려보자 항목 복제해서 서버에 전달하는 곳. 그게 바로 Replicated
	UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/Inventory.Inv_ItemManifest"), Replicated) //인벤토리 아이템 블루프린트 만드는 곳? 파생?
	FInstancedStruct ItemManifest; // instance struct? 이게 뭔데?
};

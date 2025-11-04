#pragma once
#include "CoreMinimal.h"
#include "Types/Inv_GridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"

#include "Inv_ItemManifest.generated.h"

/*
	The Item Manifest contains all of the necessary data
	for creating a new Inventory Item
*/
class UInv_InventoryItem;
struct FInv_ItemFragment;

USTRUCT(BlueprintType)
struct INVENTORY_API FInv_ItemManifest
{
	GENERATED_BODY()

	UInv_InventoryItem* Manifest(UObject* NewOuter); //새로운 인벤토리 아이템 만들 때?
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; } // 아이템 카테고리 얻기
	FGameplayTag GetItemType() const { return ItemType; } // 아이템 타입 얻기

private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct)) 
	TArray<TInstancedStruct<FInv_ItemFragment>> Fragments; // 인벤토리 아이템 배열 공간들.

	UPROPERTY(EditAnywhere, Category = "Inventory")
	EInv_ItemCategory ItemCategory{ EInv_ItemCategory::None }; // 개별 구성요소?
	

	// 게임플레이 태그 부분
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag ItemType;

};


#pragma once
#include "CoreMinimal.h"
#include "Types/Inv_GridTypes.h"

#include "Inv_ItemManifest.generated.h"

/*
	The Item Manifest contains all of the necessary data
	for creating a new Inventory Item
*/

USTRUCT(BlueprintType)
struct INVENTORY_API FInv_ItemManifest
{
	GENERATED_BODY()

	UInv_InventoryItem* Manifest(UObject* NewOuter); //���ο� �κ��丮 ������ ���� ��?
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	EInv_ItemCategory ItemCategory{ EInv_ItemCategory::None }; // ���� �������?
};
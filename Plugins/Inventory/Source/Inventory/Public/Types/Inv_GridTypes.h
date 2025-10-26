#pragma once

// Gihyeon's Inventory Project
// 인벤토리 탭 만들기
#include "Inv_GridTypes.generated.h"

UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8
{
	Equippable,
	Consumable,
	Craftable,
	Build,
	None
};
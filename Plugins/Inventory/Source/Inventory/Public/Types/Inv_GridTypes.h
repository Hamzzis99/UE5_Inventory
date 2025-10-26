#pragma once

// Gihyeon's Inventory Project
// �κ��丮 �� �����
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
#pragma once

#include "Inv_GridTypes.generated.h"

UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8 // 위젯 만들 때 정확한 이름을 작성해야해 enum으로 알겠지. 기억해.
{
	Equippable,
	Consumable,
	Craftable,
	Build,
	None
};
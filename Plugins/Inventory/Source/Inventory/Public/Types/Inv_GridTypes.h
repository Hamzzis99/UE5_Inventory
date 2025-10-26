#pragma once

#include "Inv_GridTypes.generated.h"

UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8 // ���� ���� �� ��Ȯ�� �̸��� �ۼ��ؾ��� enum���� �˰���. �����.
{
	Equippable,
	Consumable,
	Craftable,
	Build,
	None
};
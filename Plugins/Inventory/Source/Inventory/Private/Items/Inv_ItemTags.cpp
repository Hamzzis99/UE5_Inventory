#include "Items/Inv_ItemTags.h"

namespace GameItems
{
	namespace Equipment // 장비
	{
		namespace Weapons
		{
			UE_DEFINE_GAMEPLAY_TAG(Axe, "GameItems.Equipment.Weapon.Axe")
			UE_DEFINE_GAMEPLAY_TAG(Sword, "GameItems.Equipment.Weapon.Sword")
		}

		namespace Cloaks
		{
			UE_DEFINE_GAMEPLAY_TAG(RedCloak, "GameItems.Equipment.Cloaks.Redcloak")
		}
		namespace Masks
		{
			UE_DEFINE_GAMEPLAY_TAG(SteelMask, "GameItems.Equipment.Cloaks.SteelMask")
		}

	}

	namespace Consumables
	{
		namespace Potions // 포션
		{
			namespace Red
			{
				UE_DEFINE_GAMEPLAY_TAG(Small, "GameItems.Consumables.Potions.Red.Small")
				UE_DEFINE_GAMEPLAY_TAG(Large, "GameItems.Consumables.Potions.Red.Large")
			}

			namespace Blue
			{
				UE_DEFINE_GAMEPLAY_TAG(Small, "GameItems.Consumables.Potions.Blue.Small")
				UE_DEFINE_GAMEPLAY_TAG(Large, "GameItems.Consumables.Potions.Blue.Large")
			}
		}
	}

	namespace Craftables // 재료
	{
		UE_DEFINE_GAMEPLAY_TAG(FireFernFruit, "GameItems.Craftables.Blue.FireFernFruit")
		UE_DEFINE_GAMEPLAY_TAG(LuminDaisy, "GameItems.Craftables.Blue.LuminDaisy")
		UE_DEFINE_GAMEPLAY_TAG(ScorchPetalBlossom, "GameItems.Craftables.Blue.ScorchPetalBlossom")
	}

	namespace Builds // 건축
	{

	}
}


//GameItems::Equipment::Weapons::Axe // 식별자에 접근법.

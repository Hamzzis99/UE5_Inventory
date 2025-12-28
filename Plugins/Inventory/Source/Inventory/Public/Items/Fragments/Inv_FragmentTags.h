#pragma once

#include "NativeGameplayTags.h"

namespace FragmentTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GridFragment)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IconFragment)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StackableFragment)
	
	// Consumable Fragments
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ConsumableFragment)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ItemNameFragment)
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(PrimaryStatFragment)
	
	namespace StatMod // 아이템 능력치를 변경해주는 역할.
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatMod_1)	
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatMod_2)	
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatMod_3)	
	}
}
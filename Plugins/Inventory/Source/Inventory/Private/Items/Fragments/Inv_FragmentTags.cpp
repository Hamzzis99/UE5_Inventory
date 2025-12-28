#include "Items/Fragments/Inv_FragmentTags.h"

namespace FragmentTags
{
	UE_DEFINE_GAMEPLAY_TAG(GridFragment, "FragmentTags.GridFragment")
	UE_DEFINE_GAMEPLAY_TAG(IconFragment, "FragmentTags.IconFragment")
	UE_DEFINE_GAMEPLAY_TAG(StackableFragment, "FragmentTags.StackableFragment")
	UE_DEFINE_GAMEPLAY_TAG(ConsumableFragment, "FragmentTags.ConsumableFragment")
	UE_DEFINE_GAMEPLAY_TAG(ItemNameFragment, "FragmentTags.ItemNameFragment")
	
	UE_DEFINE_GAMEPLAY_TAG(PrimaryStatFragment, "FragmentTags.PrimaryStatFragment")
	
	namespace StatMod // 아이템 능력치를 변경해주는 역할.
	{
		UE_DEFINE_GAMEPLAY_TAG(StatMod_1, "StatMod.1")
		UE_DEFINE_GAMEPLAY_TAG(StatMod_2, "StatMod.2")
		UE_DEFINE_GAMEPLAY_TAG(StatMod_3, "StatMod.3")
	}
}
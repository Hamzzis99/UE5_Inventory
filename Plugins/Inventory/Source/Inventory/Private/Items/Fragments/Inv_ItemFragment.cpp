#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Composite/Inv_CompositeBase.h"
#include "Widgets/Composite/Inv_Leaf_Image.h"
#include "Widgets/Composite/Inv_Leaf_Text.h"

// 아이템 프래그먼트 동화 (확장 시키는 역할)
void FInv_InventoryItemFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;
	Composite->Expand();
}

bool FInv_InventoryItemFragment::MatchesWidgetTag(const UInv_CompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

void FInv_ImageFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite); // 위젯 태그가 일치한지 확인하는 법.
	if (!MatchesWidgetTag(Composite)) return;
	
	UInv_Leaf_Image* Image = Cast<UInv_Leaf_Image>(Composite);
	if (!IsValid(Image)) return;
	
	Image->SetImage(Icon); // 아이콘 설정
	Image->SetBoxSize(IconDimensions); // 박스 크기 설정
	Image->SetImageSize(IconDimensions); // 이미지 크기 설정
}

void FInv_TextFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;
	
	UInv_Leaf_Text* LeafText = Cast<UInv_Leaf_Text>(Composite);
	if (!IsValid(LeafText)) return;
	
	LeafText->SetText(FragmentText); // 텍스트 설정
}

void FInv_HealthPotionFragment::OnConsume(APlayerController* PC)
{
	// 1. Get a stats component from the PC or the PC->GetPawn()
	// PC 또는 PC->GetPawn()에서 Stats Component를 가져오거나
	// or get the Ability System Component and apply a Gameplay
	// Ability System Component를 통해 Gameplay Effect 적용
	// or call an interface function for Healing()
	// 힐링을 위한 인터페이스 함수 호출
	
	//디버그 메시지
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,
		FString::Printf(TEXT("Consumed Health Potion! Healed for %f HP"), HealAmount));
}

void FInv_ManaPotionFragment::OnConsume(APlayerController* PC)
{
	//Replenish mana however you wish
	
	//디버그 메시지
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Blue,
		FString::Printf(TEXT("Consumed Mana Potion! Healed for %f HP"), ManaAmount));
}

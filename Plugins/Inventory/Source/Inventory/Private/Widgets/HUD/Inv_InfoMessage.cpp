// Gihyeon's Inventory Project


#include "Widgets/HUD/Inv_InfoMessage.h"

#include "Components/TextBlock.h"

void UInv_InfoMessage::NativeConstruct()
{
	Super::NativeOnInitialized();

	Text_Message->SetText(FText::GetEmpty());
	MessageHide();
}

//메시지 출력할 때 쓰이는 CPP
void UInv_InfoMessage::SetMessage(const FText& Message)
{
	Text_Message->SetText(Message);

	if (!bIsMessageActive)
	{
		MessageShow();
	}
	bIsMessageActive = true;

	GetWorld()->GetTimerManager().SetTimer(MessageTimer, [this]() //람다를 써서 과부화를 줄이는 것.
		{
			MessageHide();
			bIsMessageActive = false;
		}, MessageLifetime, false);  // 수명 이후 사라지게 만드는 것.
}
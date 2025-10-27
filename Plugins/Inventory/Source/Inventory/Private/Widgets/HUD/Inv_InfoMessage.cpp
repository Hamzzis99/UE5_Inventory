// Gihyeon's Inventory Project


#include "Widgets/HUD/Inv_InfoMessage.h"

#include "Components/TextBlock.h"

void UInv_InfoMessage::NativeConstruct()
{
	Super::NativeOnInitialized();

	Text_Message->SetText(FText::GetEmpty());
	MessageHide();
}

//�޽��� ����� �� ���̴� CPP
void UInv_InfoMessage::SetMessage(const FText& Message)
{
	Text_Message->SetText(Message);

	if (!bIsMessageActive)
	{
		MessageShow();
	}
	bIsMessageActive = true;

	GetWorld()->GetTimerManager().SetTimer(MessageTimer, [this]() //���ٸ� �Ἥ ����ȭ�� ���̴� ��.
		{
			MessageHide();
			bIsMessageActive = false;
		}, MessageLifetime, false);  // ���� ���� ������� ����� ��.
}
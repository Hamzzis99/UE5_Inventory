// Gihyeon's Inventory Project


#include "Widgets/ItemPopUp/Inv_ItemPopUp.h"

#include "Components/Button.h"
#include "Components/Slider.h"

// 버튼 및 슬라이더 위젯들을 동적 할당 하는 부분들. (콜백 함수들 버튼 클릭 시 전달 할 수 있게 하는 것.)
void UInv_ItemPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Split->OnClicked.AddDynamic(this, &ThisClass::SplitButtonClicked);
	Button_Drop->OnClicked.AddDynamic(this, &ThisClass::DropButtonClicked);
	Button_Consume->OnClicked.AddDynamic(this, &ThisClass::ConsumeButtonClicked);
	Slider_Split->OnValueChanged.AddDynamic(this, &ThisClass::SliderValueChanged);
}

void UInv_ItemPopUp::SplitButtonClicked()
{
}

void UInv_ItemPopUp::DropButtonClicked()
{
}

void UInv_ItemPopUp::ConsumeButtonClicked()
{
}

void UInv_ItemPopUp::SliderValueChanged(float Value)
{
}
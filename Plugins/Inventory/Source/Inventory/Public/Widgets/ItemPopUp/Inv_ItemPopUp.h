// Gihyeon's Inventory Project

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemPopUp.generated.h"

/**
 * The item popup widget shows up when right-clicking on an item
 * is the inventory grid.
 * 아이템 팝업 위젯은 인벤토리 그리드에서 아이템을 우클릭할 때 나타납니다.
 */
class UButton;
class USlider;
class UTextBlock;
class USizeBox;

UCLASS()
class INVENTORY_API UInv_ItemPopUp : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Split;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Drop;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consume;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> Slider_Split; // 슬라이더 버튼 만드는 법.
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SplitAmount; // 텍스트칸 만들기.
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root; // 사이즈박스 (위젯 크기 전체를 감싸는 부분)
};

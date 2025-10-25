// Gihyeon's Inventory Project

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory|HUD")
	void ShowPickupMessage(const FString& Message);


	// 아이템 획득 메시지 표시 함수
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory|HUD")
	void HidePickupMessage();
};

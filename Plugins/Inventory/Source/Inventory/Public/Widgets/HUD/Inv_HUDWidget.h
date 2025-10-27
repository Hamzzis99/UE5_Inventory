// Gihyeon's Inventory Project

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_HUDWidget.generated.h"

/**
 * 
 */
class UInv_InfoMessage;
UCLASS()
class INVENTORY_API UInv_HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;


	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory|HUD")
	void ShowPickupMessage(const FString& Message);


	// ������ ȹ�� �޽��� ǥ�� �Լ�
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory|HUD")
	void HidePickupMessage();

	// �κ��丮 ��á�ٴ� �޽��� ����ϱ�?
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InfoMessage> InfoMessage;

	UFUNCTION()
	void OnNoRoom(); // �� ��á��
};

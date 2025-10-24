#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
class UInv_HUDWidget;

UCLASS()
class INVENTORY_API AInv_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AInv_PlayerController();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:

	void PrimaryInteract();
	void CreateHUDWidget(); // ���� ���� �Լ� ����
	void TraceForItem(); // ������ ���
	

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs; // TArray�κ��� ���� �����͸� �迭ȭ ���Ѱ����� ������ ���� �����ͷ� ���� �� �ִ�! ������

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass; // ���� ����

	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget; // ���� �ν��Ͻ�

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")	
	double TraceLength; // ���� ����

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")	
	TEnumAsByte<ECollisionChannel> ItemTraceChannel; // ���� ä��? �浿 ä��? �� ���� Enum�� ������ ����

	TWeakObjectPtr<AActor> ThisActor; // ��ü�� ���� �����ʹ� ���������� ������ �÷��ǿ� ������ ����
	TWeakObjectPtr<AActor> LastActor; // ���������� ��ȣ�ۿ��� ����
};

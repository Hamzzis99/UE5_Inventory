#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"

/**
 * 
 */
class UInv_InventoryComponent;
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

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory(); // �κ��丮 ��� �Լ�

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	//Ű ��ϵ�?
	void PrimaryInteract();
	void CreateHUDWidget(); // ���� ���� �Լ� ����
	void TraceForItem(); // ������ ���
	
	//�������Ʈ���� �κ��丮 ������Ʈ�� ���� ���� WeakObjectPtr(����)�� �����ߴٰ�? ������ ����?
	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs; // TArray�κ��� ���� �����͸� �迭ȭ ���Ѱ����� ������ ���� �����ͷ� ���� �� �ִ�! ������

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction; // ��ȣ�ۿ� �׼�

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> ToggleInventoryAction; // �κ��丮 Ű ������ �׼�

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

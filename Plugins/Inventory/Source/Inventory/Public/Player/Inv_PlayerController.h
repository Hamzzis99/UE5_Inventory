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
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:

	void PrimaryInteract();
	void CreateHUDWidget(); // 위젯 생성 함수 선언
	void TraceForItem(); // 아이템 등록
	

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs; // TArray로부터 단일 포인터를 배열화 시켜가지고 여러개 복수 포인터로 만들 수 있다! 으하하

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass; // 위젯 선언

	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget; // 위젯 인스턴스

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")	
	double TraceLength; // 추적 길이

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")	
	TEnumAsByte<ECollisionChannel> ItemTraceChannel; // 추적 채널? 충동 채널?
};

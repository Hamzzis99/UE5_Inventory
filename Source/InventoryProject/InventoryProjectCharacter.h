// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "InputActionValue.h"
#include "InventoryProjectCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(abstract)
class AInventoryProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** 카메라 붐 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** 팔로우 카메라 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:
	/** 입력 액션: 점프 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	/** 입력 액션: 이동 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	/** 입력 액션: 시선 회전 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	/** 입력 액션: 마우스 시선 회전 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseLookAction;

	/** [추가] 입력 액션: 사격 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	/** [추가] 블루프린트에서 설정할 투사체 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class AActor> ProjectileClass;

public:
	/** 생성자 */
	AInventoryProjectCharacter();  

protected:
	/** 입력 바인딩 설정 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** 입력 처리 함수들 */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	/** [추가] 사격 시작 함수 (클라이언트) */
	void OnFire(const FInputActionValue& Value);

	/** [추가] 서버에서 투사체를 생성하는 함수 (Server RPC) */
	UFUNCTION(Server, Reliable)
	void Server_Fire(FVector Location, FRotator Rotation);

public:
	/** 외부 호출용 이동/시점/점프 함수 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

public:
	/** 컴포넌트 게터 */
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InventoryProject.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AInventoryProjectCharacter::AInventoryProjectCharacter()
{
	// 캡슐 콜리전 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	   
	// 컨트롤러 회전에 맞춰 캐릭터가 회전하지 않도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터 이동 컴포넌트 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// 카메라 붐 생성
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// 팔로우 카메라 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AInventoryProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
	   
	   // 점프 바인딩
	   EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	   EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	   // 이동/시점 바인딩
	   EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInventoryProjectCharacter::Move);
	   EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInventoryProjectCharacter::Look);
	   EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AInventoryProjectCharacter::Look);

	   // [추가] 사격 바인딩
	   if (FireAction)
	   {
		   EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AInventoryProjectCharacter::OnFire);
	   }
	}
	else
	{
	   UE_LOG(LogInventoryProject, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void AInventoryProjectCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void AInventoryProjectCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AInventoryProjectCharacter::OnFire(const FInputActionValue& Value)
{
	// 1. 발사 위치: 캐릭터 위치에서 전방으로 100 유닛
	FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 100.0f);
	
	// 2. 발사 방향: 컨트롤러가 바라보는 방향 (에임 방향)
	FRotator SpawnRotation = GetControlRotation();

	// 3. 서버에 발사 요청 (RPC 호출)
	Server_Fire(SpawnLocation, SpawnRotation);
}

void AInventoryProjectCharacter::Server_Fire_Implementation(FVector Location, FRotator Rotation)
{
	if (ProjectileClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// 서버에서 투사체 소환
			World->SpawnActor<AActor>(ProjectileClass, Location, Rotation, SpawnParams);
		}
	}
}

void AInventoryProjectCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
	   const FRotator Rotation = GetController()->GetControlRotation();
	   const FRotator YawRotation(0, Rotation.Yaw, 0);

	   const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	   const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	   AddMovementInput(ForwardDirection, Forward);
	   AddMovementInput(RightDirection, Right);
	}
}

void AInventoryProjectCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
	   AddControllerYawInput(Yaw);
	   AddControllerPitchInput(Pitch);
	}
}

void AInventoryProjectCharacter::DoJumpStart() { Jump(); }
void AInventoryProjectCharacter::DoJumpEnd() { StopJumping(); }
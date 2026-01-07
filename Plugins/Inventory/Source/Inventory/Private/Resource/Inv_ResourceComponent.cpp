// Fill out your copyright notice in the Description page of Project Settings.

#include "Resource/Inv_ResourceComponent.h"
#include "GameFramework/Actor.h"

UInv_ResourceComponent::UInv_ResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UInv_ResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	LastDropHealth = MaxHealth;

	// Owner 액터의 OnTakeAnyDamage 델리게이트에 바인딩
	AActor* Owner = GetOwner();
	if (IsValid(Owner))
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UInv_ResourceComponent::OnOwnerTakeDamage);
		UE_LOG(LogTemp, Log, TEXT("[자원 컴포넌트] %s 초기화 완료 (최대 HP: %.1f)"), *Owner->GetName(), MaxHealth);
	}
}

void UInv_ResourceComponent::OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!GetOwner()->HasAuthority()) return;

	CurrentHealth -= Damage;

	UE_LOG(LogTemp, Warning, TEXT("[자원] %s 가 %.1f 데미지를 받았습니다. HP: %.1f / %.1f"), 
		*GetOwner()->GetName(), Damage, CurrentHealth, MaxHealth);

	// HP 간격마다 드롭 기능
	if (DropHealthInterval > 0.f)
	{
		// 이전 드롭 지점에서 DropHealthInterval만큼 떨어졌는지 확인
		while (LastDropHealth - CurrentHealth >= DropHealthInterval && CurrentHealth > 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("[자원] HP 간격 도달! 아이템 드롭 (간격: %.1f, 현재 HP: %.1f)"), 
				DropHealthInterval, CurrentHealth);
			SpawnDroppedResources();
			LastDropHealth -= DropHealthInterval;
		}
	}

	// 파괴 시 최종 드롭
	if (CurrentHealth <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("[자원] %s 파괴! 최종 자원 드롭..."), *GetOwner()->GetName());
		
		// HP 간격 드롭을 사용하지 않는 경우에만 파괴 시 드롭
		if (DropHealthInterval <= 0.f)
		{
			SpawnDroppedResources();
		}
		
		DestroyOwnerActor();
	}
}

void UInv_ResourceComponent::SpawnDroppedResources()
{
	if (!GetOwner()->HasAuthority()) return;
	if (!IsValid(DropItemClass)) 
	{
		UE_LOG(LogTemp, Error, TEXT("[자원] 드롭할 아이템 BP가 설정되지 않았습니다!"));
		return;
	}

	const int32 DropCount = FMath::RandRange(DropCountMin, DropCountMax);
	UE_LOG(LogTemp, Warning, TEXT("[자원] %d개의 아이템 소환 중: %s"), 
		DropCount, *DropItemClass->GetName());

	AActor* Owner = GetOwner();
	const FVector ActorLocation = Owner->GetActorLocation();
	const FVector ActorForward = Owner->GetActorForwardVector();

	for (int32 i = 0; i < DropCount; i++)
	{
		// 기존 SpawnDroppedItem 수학 공식 그대로 사용
		FVector RotatedForward = ActorForward.RotateAngleAxis(
			FMath::FRandRange(DropSpawnAngleMin, DropSpawnAngleMax), 
			FVector::UpVector
		);

		FVector SpawnLocation = ActorLocation + RotatedForward * FMath::FRandRange(
			DropSpawnDistanceMin, 
			DropSpawnDistanceMax
		);

		SpawnLocation.Z -= RelativeSpawnElevation;

		const FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(
			DropItemClass, 
			SpawnLocation, 
			SpawnRotation, 
			SpawnParams
		);

		if (IsValid(SpawnedItem))
		{
			UE_LOG(LogTemp, Log, TEXT("[자원] 아이템 소환 성공 %d/%d: %s (위치: %s)"), 
				i + 1, DropCount, *SpawnedItem->GetName(), *SpawnLocation.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[자원] 아이템 소환 실패 %d/%d"), i + 1, DropCount);
		}
	}
}

void UInv_ResourceComponent::DestroyOwnerActor()
{
	AActor* Owner = GetOwner();
	if (IsValid(Owner))
	{
		UE_LOG(LogTemp, Log, TEXT("[자원] Owner 액터 파괴: %s"), *Owner->GetName());
		Owner->Destroy();
	}
}


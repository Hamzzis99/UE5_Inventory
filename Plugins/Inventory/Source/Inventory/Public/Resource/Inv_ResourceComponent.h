// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_ResourceComponent.generated.h"

// 자원 채집 컴포넌트 - Static Mesh Actor에 붙여서 사용
// 오픈월드/PCG 최적화된 컴포넌트 기반 시스템
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_ResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_ResourceComponent();

	virtual void BeginPlay() override;

	// Owner 액터가 데미지를 받을 때 호출
	UFUNCTION()
	void OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

private:
	// ========== 자원 체력 설정 ==========
	
	// 자원 최대 체력 (블루프린트에서 설정 가능)
	UPROPERTY(EditAnywhere, Category = "Resource|Health", meta = (ClampMin = "1.0", DisplayName = "최대 체력"))
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "Resource|Health", meta = (DisplayName = "현재 체력"))
	float CurrentHealth;

	// HP가 이 값만큼 감소할 때마다 아이템 드롭 (0 = 파괴 시에만 드롭)
	UPROPERTY(EditAnywhere, Category = "Resource|Health", meta = (ClampMin = "0.0", DisplayName = "드롭 HP 간격 (0=파괴시만)"))
	float DropHealthInterval = 0.f;

	// 마지막으로 드롭한 HP 지점
	float LastDropHealth;

	// ========== 드롭 아이템 설정 ==========
	
	// 드롭할 아이템 Blueprint 클래스 (예: BP_Inv_FireFernFruit)
	UPROPERTY(EditAnywhere, Category = "Resource|Drop Item", meta = (DisplayName = "드롭할 아이템 BP"))
	TSubclassOf<AActor> DropItemClass;

	// 한 번에 드롭되는 아이템 개수 (최소)
	UPROPERTY(EditAnywhere, Category = "Resource|Drop Item", meta = (ClampMin = "1", DisplayName = "드롭 개수 (최소)"))
	int32 DropCountMin = 1;

	// 한 번에 드롭되는 아이템 개수 (최대)
	UPROPERTY(EditAnywhere, Category = "Resource|Drop Item", meta = (ClampMin = "1", DisplayName = "드롭 개수 (최대)"))
	int32 DropCountMax = 3;

	// ========== 드롭 위치 설정 (수학 공식) ==========
	
	// 드롭 각도 범위 (최소)
	UPROPERTY(EditAnywhere, Category = "Resource|Drop Position", meta = (DisplayName = "드롭 각도 (최소)"))
	float DropSpawnAngleMin = -85.f;

	// 드롭 각도 범위 (최대)
	UPROPERTY(EditAnywhere, Category = "Resource|Drop Position", meta = (DisplayName = "드롭 각도 (최대)"))
	float DropSpawnAngleMax = 85.f;

	// 드롭 거리 (최소)
	UPROPERTY(EditAnywhere, Category = "Resource|Drop Position", meta = (DisplayName = "드롭 거리 (최소)"))
	float DropSpawnDistanceMin = 10.f;

	// 드롭 거리 (최대)
	UPROPERTY(EditAnywhere, Category = "Resource|Drop Position", meta = (DisplayName = "드롭 거리 (최대)"))
	float DropSpawnDistanceMax = 50.f;

	// 드롭 높이 보정값
	UPROPERTY(EditAnywhere, Category = "Resource|Drop Position", meta = (DisplayName = "드롭 높이 보정"))
	float RelativeSpawnElevation = 70.f;

	// 자원 드롭 처리
	void SpawnDroppedResources();

	// Owner 액터 파괴
	void DestroyOwnerActor();
};


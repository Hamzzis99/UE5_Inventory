// Gihyeon's Inventory Project

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_BuildingComponent.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_BuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInv_BuildingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 빌드 모드 시작/종료 함수
	void StartBuildMode();
	void EndBuildMode();
	
	// 설치 액션 함수
	void TryPlaceBuilding();

	// 서버 RPC: 건물 배치 요청
	UFUNCTION(Server, Reliable)
	void Server_PlaceBuilding(TSubclassOf<AActor> BuildingClass, FVector Location, FRotator Rotation);

	// 멀티캐스트 RPC: 모든 클라이언트에게 건물 배치 알림
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnBuildingPlaced(AActor* PlacedBuilding);

	// PlayerController 약한 참조
	TWeakObjectPtr<APlayerController> OwningPC;

	// Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> BuildingMappingContext;

	// 빌드 모드 토글 액션 (고스트 메시 표시용)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_Building;

	// 설치 액션 (실제 배치)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_BuildingAction;

	// 고스트 액터 클래스 (블루프린트에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building|Ghost", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> GhostActorClass;

	// 실제 스폰된 고스트 액터 인스턴스
	UPROPERTY()
	TObjectPtr<AActor> GhostActorInstance;

	// 빌드 모드 상태
	UPROPERTY(BlueprintReadOnly, Category = "Building|State", meta = (AllowPrivateAccess = "true"))
	bool bIsInBuildMode = false;

	// 건물 설치 가능 여부 (바닥에 닿았는지, 각도가 적절한지)
	UPROPERTY(BlueprintReadOnly, Category = "Building|State", meta = (AllowPrivateAccess = "true"))
	bool bCanPlaceBuilding = false;

	// 최대 배치 거리 (플레이어로부터)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Settings", meta = (AllowPrivateAccess = "true"))
	float MaxPlacementDistance = 1000.0f;

	// 최대 허용 바닥 각도 (이보다 가파르면 설치 불가)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Settings", meta = (AllowPrivateAccess = "true"))
	float MaxGroundAngle = 45.0f;
};

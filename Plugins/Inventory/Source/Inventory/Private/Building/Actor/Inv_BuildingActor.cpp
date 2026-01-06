// Gihyeon's Inventory Project

#include "Building/Actor/Inv_BuildingActor.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AInv_BuildingActor::AInv_BuildingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 리플리케이션 활성화
	bReplicates = true;
	bAlwaysRelevant = true; // 모든 클라이언트에게 항상 관련성 있음
	SetReplicateMovement(true);

	// 건물 메시 컴포넌트 생성
	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	RootComponent = BuildingMesh;

	// 충돌 설정
	BuildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BuildingMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	BuildingMesh->SetCollisionResponseToAllChannels(ECR_Block);
}

// Called when the game starts or when spawned
void AInv_BuildingActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 서버에서만 OnBuildingPlaced 호출
	if (HasAuthority())
	{
		OnBuildingPlaced();
	}
}

// Called every frame
void AInv_BuildingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInv_BuildingActor::OnBuildingPlaced_Implementation()
{
	// 블루프린트에서 오버라이드하여 사용
	// 예: 이펙트 재생, 사운드 재생 등
	UE_LOG(LogTemp, Warning, TEXT("Building placed: %s"), *GetName());
}


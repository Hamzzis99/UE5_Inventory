#include "InventoryProjectProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AInventoryProjectProjectile::AInventoryProjectProjectile()
{
	// 멀티플레이 복제 설정
	bReplicates = true;
	SetReplicateMovement(true);

	// 1. 충돌체 설정
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->OnComponentHit.AddDynamic(this, &AInventoryProjectProjectile::OnHit);
	RootComponent = CollisionComponent;

	// 2. 메쉬 설정 (블루프린트에서 모델을 입힐 수 있게 함)
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 메쉬는 충돌 계산 제외

	// 3. 이동 컴포넌트 설정
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.f; // 속도 기본값
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// 총알 수명 (메모리 관리)
	InitialLifeSpan = 3.0f;
}

void AInventoryProjectProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AInventoryProjectProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 서버에서만 데미지 판정 처리
	if (HasAuthority() && OtherActor && (OtherActor != this) && (OtherActor != GetOwner()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, DamageValue, GetInstigatorController(), this, UDamageType::StaticClass());
		
		// 충돌 후 소멸
		Destroy();
	}
}
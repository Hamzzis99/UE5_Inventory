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

	// 2. 메쉬 설정
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 3. 이동 설정
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	InitialLifeSpan = 3.0f; // 3초 뒤 자동 소멸
}

void AInventoryProjectProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 서버에서만 데미지 처리
	if (HasAuthority() && OtherActor && (OtherActor != this) && (OtherActor != GetOwner()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, DamageValue, GetInstigatorController(), this, UDamageType::StaticClass());

		// [디버깅] 적중 로그 (빨간색)
		FString HitLog = FString::Printf(TEXT("HIT! Damage [%.1f] applied to [%s]"), DamageValue, *OtherActor->GetName());
		UE_LOG(LogTemp, Error, TEXT("%s"), *HitLog);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, HitLog);

		Destroy();
	}
}
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryProjectProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class INVENTORYPROJECT_API AInventoryProjectProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AInventoryProjectProjectile();

	/** 충돌 컴포넌트 */
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComponent;

	/** 투사체 이동 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	/** 총알 메쉬 (블루프린트에서 모델 설정 가능) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	/** 설정: 데미지 수치 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DamageValue = 20.0f;

	/** 충돌 시 호출될 함수 */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;
};
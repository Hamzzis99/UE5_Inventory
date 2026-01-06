// Gihyeon's Inventory Project


#include "Building/Components/Inv_BuildingComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UInv_BuildingComponent::UInv_BuildingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsInBuildMode = false;
	bCanPlaceBuilding = false;
	MaxPlacementDistance = 1000.0f;
	MaxGroundAngle = 45.0f; // 45도 이상 경사면에는 설치 불가
}


// Called when the game starts
void UInv_BuildingComponent::BeginPlay()
{
	Super::BeginPlay();

	// PlayerController 캐싱
	OwningPC = Cast<APlayerController>(GetOwner());
	if (!OwningPC.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Inv_BuildingComponent: Owner is not a PlayerController!"));
		return;
	}

	// 로컬 플레이어만 입력 등록
	if (!OwningPC->IsLocalController()) return;

	// Enhanced Input Subsystem에 Mapping Context 추가
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwningPC->GetLocalPlayer());
	if (IsValid(Subsystem) && IsValid(BuildingMappingContext))
	{
		Subsystem->AddMappingContext(BuildingMappingContext, 0);
		UE_LOG(LogTemp, Warning, TEXT("Building Mapping Context added successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Building Mapping Context is not set or Subsystem is invalid."));
	}

	// Input Component 바인딩
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(OwningPC->InputComponent))
	{
		if (IsValid(IA_Building))
		{
			EnhancedInputComponent->BindAction(IA_Building, ETriggerEvent::Started, this, &UInv_BuildingComponent::StartBuildMode);
			EnhancedInputComponent->BindAction(IA_Building, ETriggerEvent::Completed, this, &UInv_BuildingComponent::EndBuildMode);
			UE_LOG(LogTemp, Warning, TEXT("IA_Building action bound successfully."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("IA_Building is not set."));
		}

		if (IsValid(IA_BuildingAction))
		{
			EnhancedInputComponent->BindAction(IA_BuildingAction, ETriggerEvent::Started, this, &UInv_BuildingComponent::TryPlaceBuilding);
			UE_LOG(LogTemp, Warning, TEXT("IA_BuildingAction action bound successfully."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("IA_BuildingAction is not set."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Enhanced Input Component not found!"));
	}
}


// Called every frame
void UInv_BuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 빌드 모드일 때 고스트 메시 위치 업데이트
	if (bIsInBuildMode && IsValid(GhostActorInstance) && OwningPC.IsValid())
	{
		// 플레이어가 바라보는 방향으로 라인 트레이스
		FVector CameraLocation;
		FRotator CameraRotation;
		OwningPC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		const FVector TraceStart = CameraLocation;
		const FVector TraceEnd = TraceStart + (CameraRotation.Vector() * MaxPlacementDistance);

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwningPC->GetPawn()); // 플레이어 폰 무시
		QueryParams.AddIgnoredActor(GhostActorInstance); // 고스트 액터 자신 무시

		// 라인 트레이스 실행 (ECC_Visibility 채널 사용)
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			// 히트된 위치에 고스트 액터 배치
			GhostActorInstance->SetActorLocation(HitResult.Location);
			
			// 바닥 법선 각도 체크 - 너무 가파른 경사면인지 확인
			const FVector UpVector = FVector::UpVector;
			const float DotProduct = FVector::DotProduct(HitResult.ImpactNormal, UpVector);
			const float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(DotProduct));
			
			// 설치 가능 여부 판단
			bCanPlaceBuilding = (AngleDegrees <= MaxGroundAngle);
			
			// 디버그 라인 (빨강: 불가능, 초록: 가능)
			const FColor DebugColor = bCanPlaceBuilding ? FColor::Green : FColor::Red;
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, DebugColor, false, 0.0f, 0, 2.0f);
			DrawDebugPoint(GetWorld(), HitResult.Location, 10.0f, DebugColor, false, 0.0f);
		}
		else
		{
			// 바닥을 못 찾으면 설치 불가능
			bCanPlaceBuilding = false;
			
			// 고스트를 트레이스 끝 지점에 배치 (공중)
			GhostActorInstance->SetActorLocation(TraceEnd);
			
			// 디버그 라인 (회색: 바닥 없음)
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Silver, false, 0.0f, 0, 1.0f);
		}
	}
}

void UInv_BuildingComponent::StartBuildMode()
{
	if (!OwningPC.IsValid() || !GetWorld()) return;

	bIsInBuildMode = true;
	UE_LOG(LogTemp, Warning, TEXT("=== Build Mode STARTED ==="));

	// 고스트 액터 클래스가 설정되어 있는지 확인
	if (!GhostActorClass)
	{
		UE_LOG(LogTemp, Error, TEXT("GhostActorClass is not set! Please set it in the Blueprint."));
		return;
	}

	// 이미 고스트 액터가 있다면 제거
	if (IsValid(GhostActorInstance))
	{
		GhostActorInstance->Destroy();
		GhostActorInstance = nullptr;
	}

	// 고스트 액터 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwningPC.Get();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 플레이어 앞에 스폰
	FVector SpawnLocation = OwningPC->GetPawn()->GetActorLocation() + (OwningPC->GetPawn()->GetActorForwardVector() * 300.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	GhostActorInstance = GetWorld()->SpawnActor<AActor>(GhostActorClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (IsValid(GhostActorInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ghost Actor spawned successfully!"));
		
		// 고스트 액터의 충돌 비활성화 (선택사항)
		GhostActorInstance->SetActorEnableCollision(false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Ghost Actor!"));
	}
}

void UInv_BuildingComponent::EndBuildMode()
{
	bIsInBuildMode = false;
	UE_LOG(LogTemp, Warning, TEXT("=== Build Mode ENDED ==="));

	// 고스트 메시 제거
	if (IsValid(GhostActorInstance))
	{
		GhostActorInstance->Destroy();
		GhostActorInstance = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Ghost Actor destroyed."));
	}
}

void UInv_BuildingComponent::TryPlaceBuilding()
{
	if (!bIsInBuildMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot place building - not in build mode!"));
		return;
	}

	if (!bCanPlaceBuilding)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot place building - invalid placement location (too steep or no ground)!"));
		return;
	}

	if (!IsValid(GhostActorInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot place building - Ghost Actor is invalid!"));
		return;
	}

	if (!OwningPC.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot place building - OwningPC is invalid!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("=== TRY PLACING BUILDING (Client Request) ==="));
	
	// 고스트 액터의 현재 위치와 회전 가져오기
	const FVector BuildingLocation = GhostActorInstance->GetActorLocation();
	const FRotator BuildingRotation = GhostActorInstance->GetActorRotation();
	
	// 서버에 건물 배치 요청
	Server_PlaceBuilding(GhostActorClass, BuildingLocation, BuildingRotation);
}

void UInv_BuildingComponent::Server_PlaceBuilding_Implementation(TSubclassOf<AActor> BuildingClass, FVector Location, FRotator Rotation)
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("Server_PlaceBuilding: World is invalid!"));
		return;
	}

	if (!BuildingClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Server_PlaceBuilding: BuildingClass is invalid!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("=== SERVER PLACING BUILDING ==="));
	
	// 서버에서 실제 건물 액터 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwningPC.Get();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* PlacedBuilding = GetWorld()->SpawnActor<AActor>(BuildingClass, Location, Rotation, SpawnParams);

	if (IsValid(PlacedBuilding))
	{
		// 실제 건물이므로 충돌 활성화
		PlacedBuilding->SetActorEnableCollision(true);
		
		// 리플리케이션 활성화 (중요!)
		PlacedBuilding->SetReplicates(true);
		PlacedBuilding->SetReplicateMovement(true);
		
		UE_LOG(LogTemp, Warning, TEXT("Server: Building placed successfully at location: %s"), *Location.ToString());
		
		// 모든 클라이언트에게 건물 배치 알림 (선택사항 - 추가 로직이 필요할 때)
		Multicast_OnBuildingPlaced(PlacedBuilding);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Server: Failed to spawn building actor!"));
	}
}

void UInv_BuildingComponent::Multicast_OnBuildingPlaced_Implementation(AActor* PlacedBuilding)
{
	if (!IsValid(PlacedBuilding)) return;
	
	UE_LOG(LogTemp, Warning, TEXT("Multicast: Building placed notification received - %s"), *PlacedBuilding->GetName());
	
	// 여기에 건물 배치 후 추가 로직 (이펙트, 사운드 등) 추가 가능
}

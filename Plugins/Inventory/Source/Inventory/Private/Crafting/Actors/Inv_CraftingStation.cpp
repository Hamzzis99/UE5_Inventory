// Fill out your copyright notice in the Description page of Project Settings.

#include "Crafting/Actors/Inv_CraftingStation.h"
#include "Blueprint/UserWidget.h"
#include "Components/StaticMeshComponent.h"

AInv_CraftingStation::AInv_CraftingStation()
{
	PrimaryActorTick.bCanEverTick = false;

	// 스테이션 메시 컴포넌트 생성
	StationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StationMesh"));
	RootComponent = StationMesh;
}

void AInv_CraftingStation::BeginPlay()
{
	Super::BeginPlay();
}

void AInv_CraftingStation::OnInteract_Implementation(APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return;
	
	// 멀티플레이 체크: 로컬 컨트롤러만 UI 열기
	if (!PlayerController->IsLocalController())
	{
		UE_LOG(LogTemp, Log, TEXT("OnInteract: 로컬 컨트롤러가 아님 - UI 열기 무시"));
		return;
	}
	
	if (!IsValid(CraftingMenuClass))
	{
		UE_LOG(LogTemp, Error, TEXT("CraftingMenuClass가 설정되지 않았습니다! BP에서 위젯을 할당하세요."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("=== 크래프팅 스테이션 상호작용! ==="));
	UE_LOG(LogTemp, Warning, TEXT("Station: %s"), *GetName());
	UE_LOG(LogTemp, Warning, TEXT("Menu Class: %s"), *CraftingMenuClass->GetName());

	// 이미 메뉴가 열려있으면 닫기
	if (IsValid(CurrentCraftingMenu))
	{
		ForceCloseMenu();
		UE_LOG(LogTemp, Log, TEXT("크래프팅 메뉴 닫힘"));
		return;
	}

	// 크래프팅 메뉴 생성 및 표시
	CurrentCraftingMenu = CreateWidget<UUserWidget>(PlayerController, CraftingMenuClass);
	if (IsValid(CurrentCraftingMenu))
	{
		CurrentCraftingMenu->AddToViewport();
		
		// 입력 모드를 UI로 변경
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(CurrentCraftingMenu->TakeWidget());
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);

		// ⭐ PlayerController 저장 및 거리 체크 Timer 시작
		MenuOwnerController = PlayerController;
		GetWorld()->GetTimerManager().SetTimer(
			DistanceCheckTimerHandle,
			this,
			&AInv_CraftingStation::CheckDistanceToPlayer,
			0.5f, // 0.5초마다 체크
			true  // 반복
		);
		
		UE_LOG(LogTemp, Log, TEXT("크래프팅 메뉴 열림! 거리 체크 시작 (0.5초마다)"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("크래프팅 메뉴 생성 실패!"));
	}
}

TSubclassOf<UUserWidget> AInv_CraftingStation::GetCraftingMenuClass_Implementation() const
{
	return CraftingMenuClass;
}

float AInv_CraftingStation::GetInteractionDistance_Implementation() const
{
	return InteractionDistance;
}

void AInv_CraftingStation::CheckDistanceToPlayer()
{
	// PlayerController가 유효한지 확인
	if (!MenuOwnerController.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController가 유효하지 않음. 메뉴 강제 닫기."));
		ForceCloseMenu();
		return;
	}

	// 메뉴가 여전히 열려있는지 확인
	if (!IsValid(CurrentCraftingMenu))
	{
		UE_LOG(LogTemp, Log, TEXT("메뉴가 이미 닫혔음. Timer 정지."));
		GetWorld()->GetTimerManager().ClearTimer(DistanceCheckTimerHandle);
		MenuOwnerController.Reset();
		return;
	}

	// PlayerController의 Pawn 가져오기
	APawn* PlayerPawn = MenuOwnerController->GetPawn();
	if (!IsValid(PlayerPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn이 유효하지 않음. 메뉴 강제 닫기."));
		ForceCloseMenu();
		return;
	}

	// 거리 계산
	float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
	
	// InteractionDistance보다 멀어지면 메뉴 닫기
	if (Distance > InteractionDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어가 너무 멀어짐! (거리: %.1f / 최대: %.1f) 메뉴 자동 닫기"), Distance, InteractionDistance);
		ForceCloseMenu();
	}
}

void AInv_CraftingStation::ForceCloseMenu()
{
	// 메뉴 제거
	if (IsValid(CurrentCraftingMenu))
	{
		CurrentCraftingMenu->RemoveFromParent();
		CurrentCraftingMenu = nullptr;
	}

	// 입력 모드 복구
	if (MenuOwnerController.IsValid())
	{
		FInputModeGameOnly InputMode;
		MenuOwnerController->SetInputMode(InputMode);
		MenuOwnerController->SetShowMouseCursor(false);
	}

	// Timer 정지
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(DistanceCheckTimerHandle);
	}

	// PlayerController 참조 초기화
	MenuOwnerController.Reset();

	UE_LOG(LogTemp, Log, TEXT("크래프팅 메뉴 강제 닫힘. Timer 정지됨."));
}

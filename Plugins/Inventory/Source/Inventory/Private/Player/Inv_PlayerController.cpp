#include "Player/Inv_PlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Widgets/HUD/Inv_HUDWidget.h"

AInv_PlayerController::AInv_PlayerController()
{
	// 추적길이 초기화
	PrimaryActorTick.bCanEverTick = true;
	TraceLength = 500.0;
}

void AInv_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TraceForItem();
}

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		for (UInputMappingContext* CurrentContext : DefaultIMCs)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}

	CreateHUDWidget(); // 위젯 생성 호출
}

void AInv_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &AInv_PlayerController::PrimaryInteract);
}

void AInv_PlayerController::PrimaryInteract()
{
	UE_LOG(LogTemp, Log, TEXT("Primary Interact"))
}

void AInv_PlayerController::CreateHUDWidget() // 위젯 생성 부분
{
	if (!IsLocalController()) return;
	HUDWidget = CreateWidget<UInv_HUDWidget>(this, HUDWidgetClass);
	if(IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void AInv_PlayerController::TraceForItem()
{
	// 바라보는 뷰포트 좌표 반환
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f; // 뷰포트 중앙 좌표

	FVector TraceStart;
	FVector Forward; //당연히 법선벡터
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, Forward)) return;

	const FVector TraceEnd = TraceStart + (Forward * TraceLength); //괄호는 쉽게 변경할 수 있는 매개변수	
	FHitResult HitResult;
	//라인트레이스는 한 번 봐야겠음
	//선 추적부분 기억하지 이제 매 프레임마다 라인트레이스를 한다. (ToonTanks 처럼) 
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();


	if (ThisActor == LastActor) return;

	if (ThisActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Started Tracing"))
	}
	if (LastActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Stopped tracing"))
	}
}

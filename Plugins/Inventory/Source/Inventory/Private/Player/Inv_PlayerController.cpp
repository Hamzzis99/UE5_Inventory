#include "Player/Inv_PlayerController.h"
#include "Items/Components/Inv_ItemComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/Inv_HUDWidget.h"
#include "Interaction/Inv_Highlightable.h"

AInv_PlayerController::AInv_PlayerController()
{
	// �������� �ʱ�ȭ
	PrimaryActorTick.bCanEverTick = true;
	TraceLength = 500.0;
	ItemTraceChannel = ECC_GameTraceChannel1; //Ŀ���� ä�� 1���� ���
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

	CreateHUDWidget(); // ���� ���� ȣ��
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

void AInv_PlayerController::CreateHUDWidget() // ���� ���� �κ�
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
	// �ٶ󺸴� ����Ʈ ��ǥ ��ȯ
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f; // ����Ʈ �߾� ��ǥ

	FVector TraceStart;
	FVector Forward; //�翬�� ��������
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, Forward)) return;

	const FVector TraceEnd = TraceStart + (Forward * TraceLength); //��ȣ�� ���� ������ �� �ִ� �Ű�����	
	FHitResult HitResult;
	//����Ʈ���̽��� �� �� ���߰���
	//�� �����κ� ������� ���� �� �����Ӹ��� ����Ʈ���̽��� �Ѵ�. (ToonTanks ó��) 
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();

	//�Ⱦ� �κ� ������ �ڵ��.
	if (!ThisActor.IsValid())
	{
		if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage(); //�������Ʈ���� ������ �͵�.
	}

	if (ThisActor == LastActor) return;

	if (ThisActor.IsValid())
	{
		if (UActorComponent* Highlightable = ThisActor->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			IInv_Highlightable::Execute_Highlight(Highlightable); //��� ������Ʈ?
		}

		UInv_ItemComponent* ItemComponent = ThisActor->FindComponentByClass<UInv_ItemComponent>();
		if (!IsValid(ItemComponent)) return;

		if (IsValid(HUDWidget)) HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage()); //�������Ʈ���� ������ �͵�.
		UE_LOG(LogTemp, Warning, TEXT("Started tracing"))
	}

	if (LastActor.IsValid())
	{
		if (UActorComponent* Highlightable = LastActor->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			IInv_Highlightable::Execute_UnHighlight(Highlightable); //��� ������Ʈ?
		}
	}
}

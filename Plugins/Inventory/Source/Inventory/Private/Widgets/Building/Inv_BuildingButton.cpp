// Gihyeon's Inventory Project

#include "Widgets/Building/Inv_BuildingButton.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Building/Components/Inv_BuildingComponent.h"
#include "GameFramework/PlayerController.h"

void UInv_BuildingButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 버튼 클릭 이벤트 바인딩
	if (IsValid(Button_Main))
	{
		Button_Main->OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inv_BuildingButton: Button_Main is not bound! Make sure the widget has a Button named 'Button_Main'."));
	}
}

void UInv_BuildingButton::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯이 생성될 때 UI 업데이트
	if (IsValid(Text_BuildingName))
	{
		Text_BuildingName->SetText(BuildingName);
	}

	if (IsValid(Image_Icon) && IsValid(BuildingIcon))
	{
		Image_Icon->SetBrushFromTexture(BuildingIcon);
	}
}

void UInv_BuildingButton::SetBuildingInfo(const FText& Name, UTexture2D* Icon, TSubclassOf<AActor> GhostClass, TSubclassOf<AActor> BuildingClass, int32 ID)
{
	BuildingName = Name;
	BuildingIcon = Icon;
	GhostActorClass = GhostClass;
	ActualBuildingClass = BuildingClass;
	BuildingID = ID;

	// UI 업데이트
	if (IsValid(Text_BuildingName))
	{
		Text_BuildingName->SetText(BuildingName);
	}

	if (IsValid(Image_Icon) && IsValid(BuildingIcon))
	{
		Image_Icon->SetBrushFromTexture(BuildingIcon);
	}
}

void UInv_BuildingButton::OnButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("=== BUILDING BUTTON CLICKED ==="));
	UE_LOG(LogTemp, Warning, TEXT("Building Name: %s"), *BuildingName.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Building ID: %d"), BuildingID);

	// PlayerController 가져오기
	APlayerController* PC = GetOwningPlayer();
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Error, TEXT("Inv_BuildingButton: Owning Player is invalid!"));
		return;
	}

	// BuildingComponent 가져오기
	UInv_BuildingComponent* BuildingComp = PC->FindComponentByClass<UInv_BuildingComponent>();
	if (!IsValid(BuildingComp))
	{
		UE_LOG(LogTemp, Error, TEXT("Inv_BuildingButton: BuildingComponent not found on Player Controller!"));
		return;
	}

	// Component에 건물 선택 알림
	BuildingComp->OnBuildingSelectedFromWidget(GhostActorClass, ActualBuildingClass, BuildingID);
}


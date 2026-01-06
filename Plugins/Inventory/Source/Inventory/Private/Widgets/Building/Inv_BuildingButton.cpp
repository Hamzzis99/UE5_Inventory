// Gihyeon's Inventory Project

#include "Widgets/Building/Inv_BuildingButton.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Building/Components/Inv_BuildingComponent.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/FastArray/Inv_FastArray.h"
#include "Items/Inv_InventoryItem.h"
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

	// 초기 버튼 상태 업데이트
	UpdateButtonState();
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

	// 재료 체크
	if (!HasRequiredMaterials())
	{
		UE_LOG(LogTemp, Warning, TEXT("=== 재료가 부족합니다! ==="));
		UE_LOG(LogTemp, Warning, TEXT("필요한 재료: %s x %d"), *RequiredMaterialTag.ToString(), RequiredAmount);
		// TODO: UI 팝업 표시
		return;
	}

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

void UInv_BuildingButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 매 프레임 버튼 상태 업데이트 (재료 개수 변경 감지)
	UpdateButtonState();
}

bool UInv_BuildingButton::HasRequiredMaterials()
{
	// 재료가 설정되지 않았으면 true (재료 필요 없음)
	if (!RequiredMaterialTag.IsValid() || RequiredAmount <= 0)
	{
		return true;
	}

	// PlayerController 가져오기
	APlayerController* PC = GetOwningPlayer();
	if (!IsValid(PC)) return false;

	// InventoryComponent 가져오기
	UInv_InventoryComponent* InvComp = PC->FindComponentByClass<UInv_InventoryComponent>();
	if (!IsValid(InvComp)) return false;

	// 재료 찾기
	FInv_InventoryFastArray& InventoryList = InvComp->GetInventoryList();
	UInv_InventoryItem* Item = InventoryList.FindFirstItemByType(RequiredMaterialTag);
	if (!Item) return false; // 재료 없음

	// 개수 확인
	int32 CurrentAmount = Item->GetTotalStackCount();
	return CurrentAmount >= RequiredAmount;
}

void UInv_BuildingButton::UpdateButtonState()
{
	if (!IsValid(Button_Main)) return;

	bool bHasMaterials = HasRequiredMaterials();

	// 버튼 활성화/비활성화
	Button_Main->SetIsEnabled(bHasMaterials);

	// 텍스트 색상 변경 (재료 부족 시 빨간색)
	if (IsValid(Text_BuildingName))
	{
		FLinearColor TextColor = bHasMaterials ? FLinearColor::White : FLinearColor::Red;
		Text_BuildingName->SetColorAndOpacity(FSlateColor(TextColor));
	}
}


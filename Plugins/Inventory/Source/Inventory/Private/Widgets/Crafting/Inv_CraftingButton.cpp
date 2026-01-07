// Gihyeon's Inventory Project

#include "Widgets/Crafting/Inv_CraftingButton.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"

void UInv_CraftingButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Button_Main))
	{
		Button_Main->OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);
	}
}

void UInv_CraftingButton::NativeConstruct()
{
	Super::NativeConstruct();

	// UI 초기 설정
	if (IsValid(Text_ItemName))
	{
		Text_ItemName->SetText(ItemName);
	}

	if (IsValid(Image_ItemIcon) && IsValid(ItemIcon))
	{
		Image_ItemIcon->SetBrushFromTexture(ItemIcon);
	}

	// 인벤토리 델리게이트 바인딩
	BindInventoryDelegates();

	// 초기 버튼 상태 업데이트
	UpdateButtonState();
}

void UInv_CraftingButton::NativeDestruct()
{
	Super::NativeDestruct();

	// 델리게이트 언바인딩
	UnbindInventoryDelegates();
}

void UInv_CraftingButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInv_CraftingButton::SetCraftingInfo(const FText& Name, UTexture2D* Icon, TSubclassOf<AActor> ItemActorClassParam)
{
	ItemName = Name;
	ItemIcon = Icon;
	ItemActorClass = ItemActorClassParam;

	// UI 즉시 업데이트
	if (IsValid(Text_ItemName))
	{
		Text_ItemName->SetText(ItemName);
	}

	if (IsValid(Image_ItemIcon) && IsValid(ItemIcon))
	{
		Image_ItemIcon->SetBrushFromTexture(ItemIcon);
	}
}

void UInv_CraftingButton::OnButtonClicked()
{
	if (!HasRequiredMaterials())
	{
		UE_LOG(LogTemp, Warning, TEXT("재료가 부족합니다!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("=== 아이템 제작 시작! ==="));
	UE_LOG(LogTemp, Warning, TEXT("아이템: %s"), *ItemName.ToString());

	// TODO: 4단계에서 구현
	// 1. 재료 차감
	// 2. 아이템 생성
	// 3. 인벤토리 추가
}

bool UInv_CraftingButton::HasRequiredMaterials()
{
	UInv_InventoryComponent* InvComp = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	if (!IsValid(InvComp)) return false;

	// 재료 1 체크
	if (RequiredMaterialTag.IsValid() && RequiredAmount > 0)
	{
		int32 TotalCount = 0;
		const auto& AllItems = InvComp->GetInventoryList().GetAllItems();
		
		for (UInv_InventoryItem* Item : AllItems)
		{
			if (!IsValid(Item)) continue;
			if (!Item->GetItemManifest().GetItemType().MatchesTagExact(RequiredMaterialTag)) continue;
			
			TotalCount += Item->GetTotalStackCount();
		}

		if (TotalCount < RequiredAmount)
		{
			UE_LOG(LogTemp, Log, TEXT("재료1 부족: %d/%d (%s)"), TotalCount, RequiredAmount, *RequiredMaterialTag.ToString());
			return false;
		}
	}

	// 재료 2 체크
	if (RequiredMaterialTag2.IsValid() && RequiredAmount2 > 0)
	{
		int32 TotalCount = 0;
		const auto& AllItems = InvComp->GetInventoryList().GetAllItems();
		
		for (UInv_InventoryItem* Item : AllItems)
		{
			if (!IsValid(Item)) continue;
			if (!Item->GetItemManifest().GetItemType().MatchesTagExact(RequiredMaterialTag2)) continue;
			
			TotalCount += Item->GetTotalStackCount();
		}

		if (TotalCount < RequiredAmount2)
		{
			UE_LOG(LogTemp, Log, TEXT("재료2 부족: %d/%d (%s)"), TotalCount, RequiredAmount2, *RequiredMaterialTag2.ToString());
			return false;
		}
	}

	return true;
}

void UInv_CraftingButton::UpdateButtonState()
{
	if (!IsValid(Button_Main)) return;

	const bool bHasMaterials = HasRequiredMaterials();
	Button_Main->SetIsEnabled(bHasMaterials);

	UE_LOG(LogTemp, Log, TEXT("CraftingButton: 버튼 상태 업데이트 - %s"), bHasMaterials ? TEXT("활성화") : TEXT("비활성화"));
}

void UInv_CraftingButton::BindInventoryDelegates()
{
	UInv_InventoryComponent* InvComp = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	if (!IsValid(InvComp)) return;

	if (!InvComp->OnItemAdded.IsAlreadyBound(this, &ThisClass::OnInventoryItemAdded))
	{
		InvComp->OnItemAdded.AddDynamic(this, &ThisClass::OnInventoryItemAdded);
	}

	if (!InvComp->OnItemRemoved.IsAlreadyBound(this, &ThisClass::OnInventoryItemRemoved))
	{
		InvComp->OnItemRemoved.AddDynamic(this, &ThisClass::OnInventoryItemRemoved);
	}

	if (!InvComp->OnStackChange.IsAlreadyBound(this, &ThisClass::OnInventoryStackChanged))
	{
		InvComp->OnStackChange.AddDynamic(this, &ThisClass::OnInventoryStackChanged);
	}

	UE_LOG(LogTemp, Log, TEXT("CraftingButton: 인벤토리 델리게이트 바인딩 완료"));
}

void UInv_CraftingButton::UnbindInventoryDelegates()
{
	UInv_InventoryComponent* InvComp = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	if (!IsValid(InvComp)) return;

	InvComp->OnItemAdded.RemoveDynamic(this, &ThisClass::OnInventoryItemAdded);
	InvComp->OnItemRemoved.RemoveDynamic(this, &ThisClass::OnInventoryItemRemoved);
	InvComp->OnStackChange.RemoveDynamic(this, &ThisClass::OnInventoryStackChanged);
}

void UInv_CraftingButton::OnInventoryItemAdded(UInv_InventoryItem* Item)
{
	UE_LOG(LogTemp, Log, TEXT("CraftingButton: 아이템 추가됨! 버튼 상태 재계산..."));
	UpdateButtonState();
}

void UInv_CraftingButton::OnInventoryItemRemoved(UInv_InventoryItem* Item)
{
	UE_LOG(LogTemp, Log, TEXT("CraftingButton: 아이템 제거됨! 버튼 상태 재계산..."));
	UpdateButtonState();
}

void UInv_CraftingButton::OnInventoryStackChanged(const FInv_SlotAvailabilityResult& Result)
{
	UE_LOG(LogTemp, Log, TEXT("CraftingButton: 스택 변경됨! 버튼 상태 재계산..."));
	UpdateButtonState();
}


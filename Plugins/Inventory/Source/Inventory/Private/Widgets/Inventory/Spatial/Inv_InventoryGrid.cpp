// Gihyeon's Inventory Project



//#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"

void UInv_InventoryGrid::NativeConstruct()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer()); // 플레이어의 인벤토리 컴포넌트를 가져온다.
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem); // 델리게이트 바인딩 
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item)
{
	//아이템 그리드 체크 부분?
	if (!MatchesCategory(Item)) return;

	UE_LOG(LogTemp, Warning, TEXT("InventoryGrid::AddItem"));
}


//2차원 격자 생성 아이템칸 만든다는 뜻.
void UInv_InventoryGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns); // Tarray 지정 하는 건 알겠는데 GridSlot이거 어디서?

	for (int32 j = 0; j < Rows; ++j)
	{
		for (int32 i = 0; i < Columns; ++i)
		{
			UInv_GridSlot* GridSlot = CreateWidget<UInv_GridSlot>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);

			const FIntPoint TilePosition(i, j);
			GridSlot->SetTileIndex(UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot); // 슬롯 사용한다는 건가?
			GridCPS->SetSize(FVector2D(TileSize, TileSize)); // 사이즈 조정
			GridCPS->SetPosition(TilePosition * TileSize); // 위치 조정
			
			GridSlots.Add(GridSlot);
		}
	}
}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory; // 아이템 카테고리 비교
}

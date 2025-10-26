// Gihyeon's Inventory Project



#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"

void UInv_InventoryGrid::NativeConstruct()
{
	Super::NativeOnInitialized();

	ConstructGrid();
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

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


//2���� ���� ���� ������ĭ ����ٴ� ��.
void UInv_InventoryGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns); // Tarray ���� �ϴ� �� �˰ڴµ� GridSlot�̰� ���?

	for (int32 j = 0; j < Rows; ++j)
	{
		for (int32 i = 0; i < Columns; ++i)
		{
			UInv_GridSlot* GridSlot = CreateWidget<UInv_GridSlot>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);

			const FIntPoint TilePosition(i, j);
			GridSlot->SetTileIndex(UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot); // ���� ����Ѵٴ� �ǰ�?
			GridCPS->SetSize(FVector2D(TileSize, TileSize)); // ������ ����
			GridCPS->SetPosition(TilePosition * TileSize); // ��ġ ����
			
			GridSlots.Add(GridSlot);
		}
	}
}

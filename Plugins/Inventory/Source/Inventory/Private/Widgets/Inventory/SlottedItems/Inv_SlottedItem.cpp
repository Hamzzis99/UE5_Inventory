// Gihyeon's Inventory Project


#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"

#include "Components/Image.h"

void UInv_SlottedItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

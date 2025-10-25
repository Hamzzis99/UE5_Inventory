// Gihyeon's Inventory Project


#include "Items/Components/Inv_ItemComponent.h"

// Sets default values for this component's properties
UInv_ItemComponent::UInv_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


	PickupMessage = FString("E - Pick up");
}
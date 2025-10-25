// Gihyeon's Inventory Project

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_ItemComponent.generated.h"


//Blueprintable: 블루프린트 클래스로 만들 수 있게 하는 것.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class INVENTORY_API UInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInv_ItemComponent();

	FString GetPickupMessage() const { return PickupMessage;  }

protected:

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
};

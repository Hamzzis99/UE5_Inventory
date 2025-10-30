// Gihyeon's Inventory Project

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_ItemComponent.generated.h"


//Blueprintable: 블루프린트 클래스로 만들 수 있게 하는 것.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class INVENTORY_API UInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInv_ItemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FInv_ItemManifest GetItemManifest() const { return ItemManifest; } // 아이템 매니페스트 반환 함수

	FString GetPickupMessage() const { return PickupMessage;  }

protected:

private:
	UPROPERTY(Replicated, EditAnywhere, Category = "Inventory")
	FInv_ItemManifest ItemManifest;


	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
};

// Gihyeon's Inventory Project

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_ItemComponent.generated.h"


//Blueprintable: �������Ʈ Ŭ������ ���� �� �ְ� �ϴ� ��.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class INVENTORY_API UInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInv_ItemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FInv_ItemManifest GetItemManifest() const { return ItemManifest; } // ������ �Ŵ��佺Ʈ ��ȯ �Լ�

	FString GetPickupMessage() const { return PickupMessage;  }

protected:

private:
	UPROPERTY(Replicated, EditAnywhere, Category = "Inventory")
	FInv_ItemManifest ItemManifest;


	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
};

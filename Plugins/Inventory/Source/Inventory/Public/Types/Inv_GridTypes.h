#pragma once

#include "Inv_GridTypes.generated.h"

class UInv_InventoryItem;

UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8 // ���� ���� �� ��Ȯ�� �̸��� �ۼ��ؾ��� enum���� �˰���. �����.
{
	Equippable,
	Consumable,
	Craftable,
	Build,
	None
};

USTRUCT()
struct FInv_SlotAvailability
{
	GENERATED_BODY()

	//�ʱ�ȭ �κ��ε� �� ���� �� ���� ����?
	FInv_SlotAvailability() {}
	FInv_SlotAvailability(int32 ItemIndex, int32 Room, bool bHasItem) : Index(ItemIndex), AmountToFill(Room), bItemAtIndex(bHasItem) {}

	int32 Index{INDEX_NONE}; // �������� �󸶳� ä����
	int32 AmountToFill{ 0 }; // �󸶳� ä��� ������
	bool bItemAtIndex{ false }; // ������ ������ ������ �ϴ���
};

USTRUCT()
//� �׸��α� �������ְ� �ϴ� �κе�.
struct FInv_SlotAvailabilityResult
{
	GENERATED_BODY()

	FInv_SlotAvailabilityResult() {} //�� �� ������� ����� ����?

	
	TWeakObjectPtr<UInv_InventoryItem> Item;
	int32 TotalRoomToFill{ 0 }; // ä�� �� �ִ� ���� (0���� �Ұ���)
	int32 Remainder{ 0 };
	bool bStackable = {false }; // ���� �� �ִ���
	TArray<FInv_SlotAvailability> SlotAvailabilities; //���� ���� ���θ� ����� ��.
};
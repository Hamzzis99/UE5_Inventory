#pragma once
// Fast Array <- �̰Ŵ� ���� ����ȭ �迭�� ��Ʈ��ũ ����ȭ�� ����Ѵٰ� �ϳ׿�?

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "Inv_FastArray.generated.h"

class UInv_InventoryComponent;
class UInv_InventoryItem;
class UInv_ItemComponent;

/* A Single entry in an Inventory*/

USTRUCT(BlueprintType)
//����ü�� F��� ������ �ϴ±���?
struct FInv_InventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInv_InventoryEntry() {}

private:
	//��� ������Ҵ� friend�� ����� Ŭ���������� ���� ���� 
	//�̰� �� ���� �ؾ߰ڴµ�? friend �� ��Ծ���
	//�ƴ� ���ʿ� ����ü�ε� �� ���� private ������ ���� friend�� ��������? ��¥ �𸣰ڳ�
	friend struct FInv_InventoryFastArray;
	friend UInv_InventoryComponent;

	UPROPERTY()
	TObjectPtr<UInv_InventoryItem> Item = nullptr; // �������� �ʱ�ȭ �ؾ��� �翬��
};

/* List of inventory Items 
�κ��丮 ������ ����ϴ� ��?*/
USTRUCT(BlueprintType)
struct FInv_InventoryFastArray : public FFastArraySerializer
{
	GENERATED_BODY()

	FInv_InventoryFastArray() : OwnerComponent(nullptr) {}
	FInv_InventoryFastArray(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

	//��ƿ��Ƽ �Լ� ����
	TArray<UInv_InventoryItem*> GetAllItems() const; // ������ ���� ������

	// FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize); // ���� �� ó��
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	// End of FFastArraySerializer contract
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		// �� �������.
		// �κ��丮 ���� �迭�� ���� �ν��Ͻ��� �����ؾ� �ϱ� ������ �������� ����� ��.
		return FastArrayDeltaSerialize<FInv_InventoryEntry, FInv_InventoryFastArray>(Entries, DeltaParams, *this);
	}

	UInv_InventoryItem* AddEntry(UInv_ItemComponent* ItemComponent); // �κ��丮 �׸� �߰�
	UInv_InventoryItem* AddEntry(UInv_InventoryItem* Item);
	void RemoveEntry(UInv_InventoryItem* Item); // �κ��丮 �׸� ����

private:
	//�ƴ� ����ü�ε� �� friend�� �����ϳİ�!!! ��!!
	friend UInv_InventoryComponent;

	// Replicated list of items 
	UPROPERTY()
	TArray<FInv_InventoryEntry> Entries;
	UPROPERTY(NotReplicated) // �̰Ŵ� ���� �ȵǴ� �Ӽ��̶�� ���ΰ�?
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FInv_InventoryFastArray> : public TStructOpsTypeTraitsBase2<FInv_InventoryFastArray>
{
	enum { WithNetDeltaSerializer = true }; // �� ����ü�� ��Ʈ��ũ ��Ÿ ����ȭ�� �������� ��Ÿ��
};
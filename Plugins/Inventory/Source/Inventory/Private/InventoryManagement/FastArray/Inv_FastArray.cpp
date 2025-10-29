#include "InventoryManagement/FastArray/Inv_FastArray.h"

#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Inv_InventoryItem.h"

TArray<UInv_InventoryItem*> FInv_InventoryFastArray::GetAllItems() const
{
	TArray<UInv_InventoryItem*> Results;
	Results.Reserve(Entries.Num());
	for (const auto& Entry : Entries)
	{
		if (!IsValid(Entry.Item)) continue;
		Results.Add(Entry.Item);
	}
	return Results;
}

void FInv_InventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	UInv_InventoryComponent* IC = Cast<UInv_InventoryComponent>(OwnerComponent);
	if (!IsValid(IC)) return;

	for (int32 Index : RemovedIndices)
	{
		IC->OnItemRemoved.Broadcast(Entries[Index].Item); // 아이템 제거 델리게이트 브로드캐스트 항목에 접근?
	}
}

void FInv_InventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	UInv_InventoryComponent* IC = Cast<UInv_InventoryComponent>(OwnerComponent);
	if (!IsValid(IC)) return;

	for (int32 Index : AddedIndices) // 
	{
		IC->OnItemAdded.Broadcast(Entries[Index].Item); // 브로드캐스트가 뭐였지? 까먹었어 ToonTanks 다시 봐야해?
	}
}

UInv_InventoryItem* FInv_InventoryFastArray::AddEntry(UInv_ItemComponent* ItemComponent)
{
	//TODO : Implement once ItemComponent is more complete 
	return nullptr;
}

UInv_InventoryItem* FInv_InventoryFastArray::AddEntry(UInv_InventoryItem* Item)
{
	//빠른 배열 직렬화 방침이 있다고? 강사님 입장에선? 서버에 이 복제된 배열을 덮어씌운다.
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority()); // 권한이 있는지 확인 

	FInv_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = Item;

	MarkItemDirty(NewEntry); // 복제되어야 함을 알려주는 것.
	return Item;
}

void FInv_InventoryFastArray::RemoveEntry(UInv_InventoryItem* Item)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt) // 반복자가 가리키는 항목?
	{
		FInv_InventoryEntry& Entry = *EntryIt;
		if (Entry.Item == Item)
		{
			EntryIt.RemoveCurrent(); // 현재 항목 제거
			MarkArrayDirty(); 
		}
	}
}

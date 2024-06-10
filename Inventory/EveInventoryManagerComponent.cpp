// Fill out your copyright notice in the Description page of Project Settings.


#include "EveInventoryManagerComponent.h"

#include "EveInventoryItemDefinition.h"
#include "EveInventoryItemInstance.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

FString FEveInventoryEntry::GetDebugString() const
{
	TSubclassOf<UEveInventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));
}

TArray<UEveInventoryItemInstance*> FEveInventoryList::GetAllItems() const
{
	TArray<UEveInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FEveInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

void FEveInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FEveInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FEveInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FEveInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FEveInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FEveInventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

UEveInventoryItemInstance* FEveInventoryList::AddEntry(TSubclassOf<UEveInventoryItemDefinition> ItemDef,
	int32 StackCount)
{
	UEveInventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FEveInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UEveInventoryItemInstance>(OwnerComponent->GetOwner());  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDef);
	for (UEveInventoryItemFragment* Fragment : GetDefault<UEveInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;

	//const UEveInventoryItemDefinition* ItemCDO = GetDefault<UEveInventoryItemDefinition>(ItemDef);
	MarkItemDirty(NewEntry);

	return Result;
}

void FEveInventoryList::AddEntry(UEveInventoryItemInstance* Instance)
{
	unimplemented();
}

void FEveInventoryList::RemoveEntry(UEveInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FEveInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

void FEveInventoryList::BroadcastChangeMessage(FEveInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	FEveInventoryChangeMessage Message;
	Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;

	//TODO: SEND MESSAGE
}


//////////////////////////////////////////////////////////////////////
// UEveInventoryManagerComponent

UEveInventoryManagerComponent::UEveInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer), InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

void UEveInventoryManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}


bool UEveInventoryManagerComponent::CanAddItemDefinition(TSubclassOf<UEveInventoryItemDefinition> ItemDef,
                                                         int32 StackCount)
{
	//TODO: Add support for stack limit / uniqueness checks / etc...
	return true;
}

UEveInventoryItemInstance* UEveInventoryManagerComponent::AddItemDefinition(
	TSubclassOf<UEveInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UEveInventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
		
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void UEveInventoryManagerComponent::AddItemInstance(UEveInventoryItemInstance* ItemInstance)
{
	InventoryList.AddEntry(ItemInstance);
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
}

void UEveInventoryManagerComponent::RemoveItemInstance(UEveInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UEveInventoryItemInstance*> UEveInventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

UEveInventoryItemInstance* UEveInventoryManagerComponent::FindFirstItemStackByDefinition(
	TSubclassOf<UEveInventoryItemDefinition> ItemDef) const
{
	for (const FEveInventoryEntry& Entry : InventoryList.Entries)
	{
		UEveInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

int32 UEveInventoryManagerComponent::GetTotalItemCountByDefinition(
	TSubclassOf<UEveInventoryItemDefinition> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FEveInventoryEntry& Entry : InventoryList.Entries)
	{
		UEveInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				++TotalCount;
			}
		}
	}

	return TotalCount;
}

bool UEveInventoryManagerComponent::ConsumeItemsByDefinition(TSubclassOf<UEveInventoryItemDefinition> ItemDef,
	int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	//@TODO: N squared right now as there's no acceleration structure
	int32 TotalConsumed = 0;
	while (TotalConsumed < NumToConsume)
	{
		if (UEveInventoryItemInstance* Instance = UEveInventoryManagerComponent::FindFirstItemStackByDefinition(ItemDef))
		{
			InventoryList.RemoveEntry(Instance);
			++TotalConsumed;
		}
		else
		{
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

bool UEveInventoryManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FEveInventoryEntry& Entry : InventoryList.Entries)
	{
		UEveInventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UEveInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing UEveInventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FEveInventoryEntry& Entry : InventoryList.Entries)
		{
			UEveInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

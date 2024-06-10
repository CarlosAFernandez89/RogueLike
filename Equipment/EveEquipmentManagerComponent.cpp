// Fill out your copyright notice in the Description page of Project Settings.


#include "EveEquipmentManagerComponent.h"

#include "AbilitySystemGlobals.h"
#include "Engine/ActorChannel.h"
#include "EveEquipmentDefinition.h"
#include "EveEquipmentInstance.h"
#include "Abilities/GSCAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

class FLifetimeProperty;
struct FReplicationFlags;


FString FEveAppliedEquipmentEntry::GetDebugString() const
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(EquipmentDefinition.Get()));

}

void FEveEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FEveAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
	}
}

void FEveEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FEveAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FEveEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
}

UEveEquipmentInstance* FEveEquipmentList::AddEntry(TSubclassOf<UEveEquipmentDefinition> EquipmentDefinition)
{
	UEveEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	
	const UEveEquipmentDefinition* EquipmentCDO = GetDefault<UEveEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UEveEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UEveEquipmentInstance::StaticClass();
	}
	
	FEveAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UEveEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);  //@TODO: Using the actor instead of component as the outer due to UE-127172
	Result = NewEntry.Instance;

	if (UGSCAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const TObjectPtr<const UGSCAbilitySet>& AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			FGSCAbilitySetHandle Handle;
			Handle.Abilities = NewEntry.GrantedHandles.AbilitySpecHandles;
			Handle.EffectHandles = NewEntry.GrantedHandles.GameplayEffectHandles;
			Handle.Attributes = NewEntry.GrantedHandles.GrantedAttributeSets;
			
			AbilitySet->GrantToAbilitySystem(ASC, Handle);
		}
	}
	else
	{
		//@TODO: Warning logging?
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);


	MarkItemDirty(NewEntry);

	return Result;
}

void FEveEquipmentList::RemoveEntry(UEveEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FEveAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (UGSCAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();
			

			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UGSCAbilitySystemComponent* FEveEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UGSCAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

//////////////////////////////////////////////////////////////////////
// UEveEquipmentManagerComponent
UEveEquipmentManagerComponent::UEveEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}



void UEveEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

UEveEquipmentInstance* UEveEquipmentManagerComponent::EquipItem(
	TSubclassOf<UEveEquipmentDefinition> EquipmentClass)
{
	UEveEquipmentInstance* Result = nullptr;
	if (EquipmentClass != nullptr)
	{
		Result = EquipmentList.AddEntry(EquipmentClass);
		if (Result != nullptr)
		{
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	return Result;
}

void UEveEquipmentManagerComponent::UnequipItem(UEveEquipmentInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

bool UEveEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FEveAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		UEveEquipmentInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UEveEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UEveEquipmentManagerComponent::UninitializeComponent()
{
	TArray<UEveEquipmentInstance*> AllEquipmentInstances;

	// gathering all instances before removal to avoid side effects affecting the equipment list iterator	
	for (const FEveAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (UEveEquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

void UEveEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing EveEquipmentInstances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FEveAppliedEquipmentEntry& Entry : EquipmentList.Entries)
		{
			UEveEquipmentInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

UEveEquipmentInstance* UEveEquipmentManagerComponent::GetFirstInstanceOfType(
	TSubclassOf<UEveEquipmentInstance> InstanceType)
{
	for (FEveAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UEveEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<UEveEquipmentInstance*> UEveEquipmentManagerComponent::GetEquipmentInstancesOfType(
	TSubclassOf<UEveEquipmentInstance> InstanceType) const
{
	TArray<UEveEquipmentInstance*> Results;
	for (const FEveAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UEveEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}

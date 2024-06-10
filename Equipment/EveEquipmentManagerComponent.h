// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Abilities/GSCAbilitySet.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ProjectEve/AbilitySystem/FEveAbilitySet_GrantedHandles.h"

#include "EveEquipmentManagerComponent.generated.h"

class UActorComponent;
class UGSCAbilitySystemComponent;
class UEveEquipmentDefinition;
class UEveEquipmentInstance;
class UEveEquipmentManagerComponent;
class UObject;
struct FFrame;
struct FEveEquipmentList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FEveAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FEveAppliedEquipmentEntry()
	{}

	FString GetDebugString() const;

private:
	friend FEveEquipmentList;
	friend UEveEquipmentManagerComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<UEveEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UEveEquipmentInstance> Instance = nullptr;

	// Authority-only list of granted handles
	UPROPERTY(NotReplicated)
	FEveAbilitySet_GrantedHandles GrantedHandles;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FEveEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FEveEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FEveEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FEveAppliedEquipmentEntry, FEveEquipmentList>(Entries, DeltaParms, *this);
	}

	UEveEquipmentInstance* AddEntry(TSubclassOf<UEveEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UEveEquipmentInstance* Instance);

private:
	UGSCAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UEveEquipmentManagerComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FEveAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FEveEquipmentList> : public TStructOpsTypeTraitsBase2<FEveEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};










/**
 * Manages equipment applied to a pawn
 */
UCLASS(BlueprintType, Const)
class UEveEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UEveEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UEveEquipmentInstance* EquipItem(TSubclassOf<UEveEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UEveEquipmentInstance* ItemInstance);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UEveEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UEveEquipmentInstance> InstanceType);

 	/** Returns all equipped instances of a given type, or an empty array if none are found */
 	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UEveEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UEveEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

	FEveEquipmentList& GetEquipmentList()
	{
		return EquipmentList;
	}

private:
	UPROPERTY(Replicated)
	FEveEquipmentList EquipmentList;
};

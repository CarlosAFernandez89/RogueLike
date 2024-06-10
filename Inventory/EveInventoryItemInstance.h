// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEve/System/GameplayTagStack.h"

#include "UObject/Object.h"
#include "EveInventoryItemInstance.generated.h"


class FLifetimeProperty;

class UEveInventoryItemDefinition;
class UEveInventoryItemFragment;
struct FFrame;
struct FGameplayTag;

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTEVE_API UEveInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UEveInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category= Inventory)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category=Inventory)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool HasStatTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UEveInventoryItemDefinition> GetItemDef() const
	{
		return ItemDef;
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(DeterminesOutputType=FragmentClass))
	const UEveInventoryItemFragment* FindFragmentByClass(TSubclassOf<UEveInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return static_cast<const ResultClass*>(FindFragmentByClass(ResultClass::StaticClass()));
	}

	void SetItemDef(TSubclassOf<UEveInventoryItemDefinition> InDef);

	friend struct FEveInventoryList;

private:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	// The item definition
	UPROPERTY(Replicated)
	TSubclassOf<UEveInventoryItemDefinition> ItemDef;
};

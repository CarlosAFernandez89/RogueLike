// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EveInventoryItemDefinition.generated.h"


template <typename T> class TSubclassOf;
class UEveInventoryItemInstance;
struct FFrame;

//////////////////////////////////////////////////////////////////////

// Represents a fragment of an item definition
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class PROJECTEVE_API UEveInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UEveInventoryItemInstance* Instance) const {}
};

//////////////////////////////////////////////////////////////////////


/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract)
class UEveInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UEveInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UEveInventoryItemFragment>> Fragments;

public:
	const UEveInventoryItemFragment* FindFragmentByClass(TSubclassOf<UEveInventoryItemFragment> FragmentClass) const;
	
};

UCLASS()
class UEveInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UEveInventoryItemFragment* FindItemDefinitionFragment(TSubclassOf<UEveInventoryItemDefinition> ItemDef, TSubclassOf<UEveInventoryItemFragment> FragmentClass);
	
};

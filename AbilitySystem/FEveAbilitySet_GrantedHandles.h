#pragma once

#include "ActiveGameplayEffectHandle.h"
#include "Engine/DataAsset.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"

#include "GameplayAbilitySpecHandle.h"
#include "Abilities/GSCAbilitySet.h"

#include "FEveAbilitySet_GrantedHandles.generated.h"

class UGSCAttributeSet;
class UGSCAbilitySystemComponent;
class UAttributeSet;

/**
 * FEveAbilitySet_GrantedHandles
 *
 *	Data used to store handles to what has been granted by the ability set.
 */
USTRUCT(BlueprintType)
struct FEveAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);


	
	void TakeFromAbilitySystem(UGSCAbilitySystemComponent* GSCASC);


	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// Pointers to the granted attribute sets
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

UCLASS(BlueprintType, Const)
class UEveAbilitySet : public UGSCAbilitySet
{
	GENERATED_BODY()

public:

	UEveAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
};

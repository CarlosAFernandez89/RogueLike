// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GSCGameplayAbility.h"
#include "EveGameplayAbility_Dodge.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTEVE_API UEveGameplayAbility_Dodge : public UGSCGameplayAbility
{
	GENERATED_BODY()

public:
	UEveGameplayAbility_Dodge();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Montages")
	TObjectPtr<UAnimMontage> Montages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Montages")
	float Rate = 1.f;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};

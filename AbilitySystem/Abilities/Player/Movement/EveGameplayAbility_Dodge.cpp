// Fill out your copyright notice in the Description page of Project Settings.


#include "EveGameplayAbility_Dodge.h"

UEveGameplayAbility_Dodge::UEveGameplayAbility_Dodge()
{
}

void UEveGameplayAbility_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}


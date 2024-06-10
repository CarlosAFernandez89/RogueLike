// Fill out your copyright notice in the Description page of Project Settings.


#include "EveCalculation_StaminaCost.h"

UEveCalculation_StaminaCost::UEveCalculation_StaminaCost()
{
}

float UEveCalculation_StaminaCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;

	const float Costs = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Stamina")), true, 0.f);

	return Costs > 0.f ? -Costs : 0.f;
}

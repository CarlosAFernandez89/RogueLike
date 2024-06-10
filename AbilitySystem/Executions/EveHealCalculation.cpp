// Fill out your copyright notice in the Description page of Project Settings.


#include "EveHealCalculation.h"

#include "Abilities/Attributes/GSCAttributeSet.h"

struct FHealStatics
{
	//Capture Declarations
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FHealStatics()
	{
		//Capture Defs for attributes.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGSCAttributeSet, Health, Target, false);
	}
};

static FHealStatics& HealStatics()
{
	static FHealStatics Statics;
	return Statics;
}

UEveHealCalculation::UEveHealCalculation()
{
	RelevantAttributesToCapture.Add(HealStatics().HealthDef);
}

void UEveHealCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float BaseHeal = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Heal")), false, -1.f), 0.f);

	if(BaseHeal < 0.f)
	{
		BaseHeal = 0.f;
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealStatics().HealthProperty, EGameplayModOp::Additive, BaseHeal));

}

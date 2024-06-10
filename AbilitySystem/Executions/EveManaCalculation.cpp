// Fill out your copyright notice in the Description page of Project Settings.


#include "EveManaCalculation.h"

#include "Abilities/Attributes/GSCAttributeSet.h"

struct FManaStatics
{
	//Capture Declarations
	DECLARE_ATTRIBUTE_CAPTUREDEF(Mana);

	FManaStatics()
	{
		//Capture Defs for attributes.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGSCAttributeSet, Mana, Target, false);
	}
};

static FManaStatics& ManaStatics()
{
	static FManaStatics Statics;
	return Statics;
}

UEveManaCalculation::UEveManaCalculation()
{
	RelevantAttributesToCapture.Add(FManaStatics().ManaDef);
}


void UEveManaCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
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
	
	float BaseManaHeal = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Mana")), false, -1.f), 0.f);

	if(BaseManaHeal < 0.f)
	{
		BaseManaHeal = 0.f;
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(ManaStatics().ManaProperty, EGameplayModOp::Additive, BaseManaHeal));

}

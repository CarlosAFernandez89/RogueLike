// Fill out your copyright notice in the Description page of Project Settings.


#include "EveDamageCalculation.h"

#include "Abilities/Attributes/GSCAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectEve/AbilitySystem/AttributeSets/CombatModifiers.h"
#include "ProjectEve/Character/EveCharacter.h"
#include "ProjectEve/NPC/EveAICharacter.h"
#include "ProjectEve/NPC/EveAIHelpers.h"

struct FDamageStatics
{
	//Capture Declarations

	//Target Attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageReduction)
	
	//Source Attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(ExtraDamageAdditive)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ExtraDamageMultiplier)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DoubleDamageChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalStrikeChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalStrikeDamageMultiplier)


	FDamageStatics()
	{
		//Capture Defs for attributes.

		//Target Attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGSCAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatModifiers, DamageReduction, Target, false);


		//Source Attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatModifiers, ExtraDamageAdditive, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatModifiers, ExtraDamageMultiplier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatModifiers, DoubleDamageChance, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatModifiers, CriticalStrikeChance, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatModifiers, CriticalStrikeDamageMultiplier, Source, true);
	}
};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UEveDamageCalculation::UEveDamageCalculation()
{
	//Target
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageReductionDef);
	
	//Source
	RelevantAttributesToCapture.Add(DamageStatics().ExtraDamageAdditiveDef);
	RelevantAttributesToCapture.Add(DamageStatics().ExtraDamageMultiplierDef);
	RelevantAttributesToCapture.Add(DamageStatics().DoubleDamageChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalStrikeChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalStrikeDamageMultiplierDef);
	
}

void UEveDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
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
	
	float BaseDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Damage")), false, -1.f), 0.f);

	// Retrieve additional attributes
	// Source
	float ExtraDamageAdditive = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ExtraDamageAdditiveDef, EvaluateParameters, ExtraDamageAdditive);
	float ExtraDamageMultiplier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ExtraDamageMultiplierDef, EvaluateParameters, ExtraDamageMultiplier);
	float DoubleDamageChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DoubleDamageChanceDef, EvaluateParameters, DoubleDamageChance);
	float CriticalStrikeChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalStrikeChanceDef, EvaluateParameters, CriticalStrikeChance);
	float CriticalStrikeDamageMultiplier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalStrikeDamageMultiplierDef, EvaluateParameters, CriticalStrikeDamageMultiplier);

	// Target
	float DamageReduction = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageReductionDef, EvaluateParameters, DamageReduction);

	// Apply additive and multiplicative damage modifiers
	BaseDamage += ExtraDamageAdditive;
	BaseDamage *= ExtraDamageMultiplier;
	
	if(BaseDamage < 0.f)
	{
		BaseDamage = 0.f;
	}
	
	// Determine if a critical strike occurs
	const bool CriticalStrike = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f) <= CriticalStrikeChance;
	if (CriticalStrike)
	{
		BaseDamage *= CriticalStrikeDamageMultiplier;
	}

	// Apply double damage based on chance
	if (UKismetMathLibrary::RandomFloatInRange(0.f, 1.f) <= DoubleDamageChance)
	{
		BaseDamage *= 2.0f;
	}
	
	// Apply damage reduction
	BaseDamage *= (1.0f - DamageReduction);

	//Round to nearest int
	BaseDamage = UKismetMathLibrary::FCeil(BaseDamage);

	if(TargetActor->IsA<AEveAICharacter>())
	{
		IEveAIHelpers::Execute_OnDamageTaken(TargetActor, BaseDamage, CriticalStrike);
	}

	if(SourceActor->IsA<AEveCharacter>())
	{
		if (CriticalStrike)
		{
			IEveCharacterHelper::Execute_OnCriticalDamageDone(SourceActor, BaseDamage);
		}
		else
		{
			IEveCharacterHelper::Execute_OnNormalDamageDone(SourceActor, BaseDamage);
		}
	}

	if(TargetActor->IsA<AEveCharacter>())
	{
		if(SourceActor != TargetActor)
		{
			// Execute damage taken on AEveCharacter 
			IEveCharacterHelper::Execute_OnDamageTaken(TargetActor, BaseDamage, SourceActor);
		}
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -BaseDamage));
}

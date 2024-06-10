// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayModMagnitudeCalculation.h"
#include "EveCalculation_StaminaCost.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTEVE_API UEveCalculation_StaminaCost : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	
	UEveCalculation_StaminaCost();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition StaminaRegDef;
};

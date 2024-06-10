// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "EveManaCalculation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTEVE_API UEveManaCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UEveManaCalculation();

protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};

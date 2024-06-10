// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GSCGameplayAbility.h"
#include "GameplayAbility_TargetLocation_Radius.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTEVE_API UGameplayAbility_TargetLocation_Radius : public UGSCGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGameplayAbility_TargetLocation_Radius();

	UFUNCTION(BlueprintCallable, Category="Targeting")
	bool GetHitLocationUnderCursor(FVector& OutHitLocation) const;
	
};

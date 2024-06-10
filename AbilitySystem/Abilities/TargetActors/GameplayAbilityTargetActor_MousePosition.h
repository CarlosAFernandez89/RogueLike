// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_GroundTrace.h"
#include "GameplayAbilityTargetActor_MousePosition.generated.h"

UCLASS(Abstract, Blueprintable, notplaceable, config=Game)
class PROJECTEVE_API AGameplayAbilityTargetActor_MousePosition : public AGameplayAbilityTargetActor_GroundTrace
{
	GENERATED_BODY()

protected:

	virtual FHitResult PerformTrace(AActor* InSourceActor) override;
};

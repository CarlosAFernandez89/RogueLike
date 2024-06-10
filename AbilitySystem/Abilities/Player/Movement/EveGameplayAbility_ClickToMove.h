// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Abilities/GSCGameplayAbility.h"
#include "EveGameplayAbility_ClickToMove.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTEVE_API UEveGameplayAbility_ClickToMove : public UGSCGameplayAbility
{
	GENERATED_BODY()

public:
	UEveGameplayAbility_ClickToMove();

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	FVector MoveToLocation();


private:
	

	UFUNCTION()
	bool GetHitLocationUnderCursor(FVector& OutHitLocation) const;


	UPROPERTY()
	FVector TargetLocation;


};

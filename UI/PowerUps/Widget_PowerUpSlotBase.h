// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ProjectEve/AbilitySystem/Abilities/AbilityPowerUps.h"

#include "Widget_PowerUpSlotBase.generated.h"

struct FPowerUpGameplayAbility;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPowerUpSlotSelected, UWidget_PowerUpSlotBase*, SlotReference);

/**
 * 
 */
UCLASS()
class PROJECTEVE_API UWidget_PowerUpSlotBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UWidget_PowerUpSlotBase();

	UFUNCTION(BlueprintNativeEvent)
	void SelectSlot();
	
	UFUNCTION(BlueprintNativeEvent)
	void DeSelectSlot();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GivePowerUp();

	UFUNCTION(BlueprintNativeEvent)
	void SetupSlot(
		FPowerUpGameplayAbility PowerUpGameplayAbility = FPowerUpGameplayAbility(),
		FPowerUpGameplayEffect PowerUpGameplayEffect = FPowerUpGameplayEffect(),
		FPowerUpGameplayTag PowerUpGameplayTag = FPowerUpGameplayTag()
		);

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPowerUpSlotSelected OnPowerUpSlotSelected;
	
};

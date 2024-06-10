// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GSCGameplayAbility.h"
#include "EveGameplayAbility_FromEquipment.generated.h"

class UEveInventoryItemInstance;
class UEveEquipmentInstance;
/**
 * 
 */
UCLASS()
class PROJECTEVE_API UEveGameplayAbility_FromEquipment : public UGSCGameplayAbility
{
	GENERATED_BODY()

public:

	UEveGameplayAbility_FromEquipment();

	UFUNCTION(BlueprintCallable, Category="Eve|Ability")
	UEveEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "Eve|Ability")
	UEveInventoryItemInstance* GetAssociatedItem() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};

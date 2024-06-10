// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EveWeaponInstance.h"
#include "EveRangedWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTEVE_API UEveRangedWeaponInstance : public UEveWeaponInstance
{
	GENERATED_BODY()
	
	UEveRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:

	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
};

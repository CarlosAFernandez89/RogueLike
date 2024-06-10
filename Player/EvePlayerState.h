// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularPlayerState.h"
#include "ProjectEve/Character/EveCharacterHelper.h"
#include "EvePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTEVE_API AEvePlayerState : public AGSCModularPlayerState, public IEveCharacterHelper
{
	GENERATED_BODY()

public:

	virtual UEveEquipmentInstance* GetEquippedItem_Implementation() override;

protected:
	
};

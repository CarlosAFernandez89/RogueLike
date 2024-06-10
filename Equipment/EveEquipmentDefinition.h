// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Templates/SubclassOf.h"

#include "EveEquipmentDefinition.generated.h"

class UGSCAbilitySet;
class AActor;
class UAttributeSet;
class UEveEquipmentInstance;

USTRUCT()
struct FEveEquipmentActorToSpawn
{
	GENERATED_BODY()

	FEveEquipmentActorToSpawn()
	{}

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};


/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class PROJECTEVE_API UEveEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UEveEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UEveEquipmentInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<TObjectPtr<const UGSCAbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FEveEquipmentActorToSpawn> ActorsToSpawn;
};

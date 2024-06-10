// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EveAIHelpers.generated.h"



UENUM(BlueprintType)
enum EAIMovementSpeed : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Patrol UMETA(DisplayName = "Patrol"),
	Jog UMETA(DisplayName = "Jog"),
	Chase UMETA(DisplayName = "Chase")
};

UENUM(BlueprintType)
enum EAISense : uint8
{
	None UMETA(DisplayName = "None"),
	Sight UMETA(DisplayName = "Sight"),
	Hearing UMETA(DisplayName = "Hearing"),
	Damage UMETA(DisplayName = "Damage")
};

UENUM(BlueprintType)
enum EAIStates : uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Attacking UMETA(DisplayName = "Attacking"),
	Frozen UMETA(DisplayName = "Frozen"),
	Investigating UMETA(DisplayName = "Investigating"),
	Dead UMETA(DisplayName = "Dead"),
	Seeking UMETA(DisplayName = "Seeking")
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEveAIHelpers : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTEVE_API IEveAIHelpers
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AIHelpers")
	class AAIPatrolRoute* GetAIPatrolRoute();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AIHelpers")
	float SetAIMovementSpeed(EAIMovementSpeed MovementSpeed);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AIHelpers")
	void GetAIIdealRange(float& AttackRadius, float& DefendRadius);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AIHelpers")
	float GetAICurrentHealth();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AIHelpers")
	float GetAIMaxHealth();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AIHelpers")
	bool GetAIIsDead();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AIHelpers")
	void AIStoreAttackTokens(AActor* AttackTarget, int32 Amount);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AIHelpers")
	void AIReturnAllAttackTokens();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AIHelpers")
	void CheckFocusTargetState();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AIHelpers")
	void NavLink_JumpToDestination(FVector Destination);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AIHelpers")
	void OnDamageTaken(float Damage, bool bCriticalHit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AIHelpers")
	void DestroyAiActor();
};

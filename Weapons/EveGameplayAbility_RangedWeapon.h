// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEve/Equipment/EveGameplayAbility_FromEquipment.h"
#include "EveGameplayAbility_RangedWeapon.generated.h"

enum ECollisionChannel : int;

class APawn;
class UEveRangedWeaponInstance;
class UObject;
struct FCollisionQueryParams;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;
struct FGameplayTag;
struct FGameplayTagContainer;

class UEveRangedWeaponInstance;
/**
 * 
 */
UCLASS()
class PROJECTEVE_API UEveGameplayAbility_RangedWeapon : public UEveGameplayAbility_FromEquipment
{
	GENERATED_BODY()

public:

	UEveGameplayAbility_RangedWeapon();

	UFUNCTION(BlueprintCallable, Category="Eve|Ability")
	UEveRangedWeaponInstance* GetWeaponInstance() const;

	UFUNCTION(BlueprintCallable, Category="Eve|Ability")
	TArray<FRotator> CalculateProjectileDirection(const FVector& ForwardDirection, int NumberOfProjectiles = 1, float SpreadDegrees = 15.f);

	//~UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGameplayAbility interface

};

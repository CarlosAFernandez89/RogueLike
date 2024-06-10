// Fill out your copyright notice in the Description page of Project Settings.


#include "EveGameplayAbility_RangedWeapon.h"

#include "AbilitySystemComponent.h"
#include "EveRangedWeaponInstance.h"
#include "Kismet/KismetMathLibrary.h"

UEveGameplayAbility_RangedWeapon::UEveGameplayAbility_RangedWeapon()
{
}

UEveRangedWeaponInstance* UEveGameplayAbility_RangedWeapon::GetWeaponInstance() const
{
	return Cast<UEveRangedWeaponInstance>(GetAssociatedEquipment());
}

TArray<FRotator> UEveGameplayAbility_RangedWeapon::CalculateProjectileDirection(const FVector& ForwardDirection, int NumberOfProjectiles, float SpreadDegrees)
{
	TArray<FRotator> ProjectileRotations;

	if (NumberOfProjectiles <= 1)
	{
		ProjectileRotations.Add(UKismetMathLibrary::MakeRotFromX(ForwardDirection));
		return ProjectileRotations; // Return empty array if no projectiles are to be spawned
	}
	

	// Calculate the initial angle
	float InitialAngle = NumberOfProjectiles % 2 == 1 ? 0.0f : SpreadDegrees / 2.0f;

	for (int i = 0; i < NumberOfProjectiles; ++i)
	{
		// Determine the direction to apply the current angle (alternate left and right)
		float CurrentAngle = InitialAngle + (i / 2) * SpreadDegrees;
		if (i % 2 == 0)
		{
			// Even index: right
			CurrentAngle = InitialAngle + (i / 2) * SpreadDegrees;
		}
		else
		{
			// Odd index: left
			CurrentAngle = -InitialAngle - (i / 2) * SpreadDegrees;
		}

		// Calculate rotation for the current projectile
		FRotator ProjectileRotation = FRotator(0.0f, CurrentAngle, 0.0f);
		FVector Direction = ProjectileRotation.RotateVector(ForwardDirection);
		FRotator DirectionRotator = Direction.Rotation();

		ProjectileRotations.Add(DirectionRotator);
	}

	return ProjectileRotations;
}

bool UEveGameplayAbility_RangedWeapon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                          const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if(bResult)
	{
		if (GetWeaponInstance() == nullptr)
		{
			UE_LOG(LogAbilitySystemComponent, Error, TEXT("Weapon ability %s cannot be activated because there is no associated ranged weapon (equipment instance=%s but needs to be derived from %s)"),
				*GetPathName(),
				*GetPathNameSafe(GetAssociatedEquipment()),
				*UEveRangedWeaponInstance::StaticClass()->GetName());
			bResult = false;
		}
	}
	return bResult;
}

void UEveGameplayAbility_RangedWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEveGameplayAbility_RangedWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
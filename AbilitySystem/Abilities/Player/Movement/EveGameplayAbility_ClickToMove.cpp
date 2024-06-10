// Fill out your copyright notice in the Description page of Project Settings.


#include "EveGameplayAbility_ClickToMove.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "ProjectEve/Character/EveCharacterHelper.h"


UEveGameplayAbility_ClickToMove::UEveGameplayAbility_ClickToMove()
{
}

void UEveGameplayAbility_ClickToMove::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEveGameplayAbility_ClickToMove::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FVector UEveGameplayAbility_ClickToMove::MoveToLocation()
{
	// Get the location from the mouse cursor
	FVector HitLocation;
	if (GetHitLocationUnderCursor(HitLocation))
	{
		IEveCharacterHelper::Execute_ClearInteractionDelegate(GetAvatarActorFromActorInfo());

		if (AController* Controller = Cast<AController>(GetAvatarActorFromActorInfo()->GetInstigatorController()))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, HitLocation);
		}
	}
	
	return HitLocation;
}



bool UEveGameplayAbility_ClickToMove::GetHitLocationUnderCursor(FVector& OutHitLocation) const
{
	if (const APlayerController* PC = Cast<APlayerController>(GetAvatarActorFromActorInfo()->GetInstigatorController()))
	{
		FHitResult HitResult;
		PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

		if (HitResult.bBlockingHit)  // Ensure there was a hit
		{
			// Get the navigation system and the current navigation data
			if (const UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
			{				
				FNavLocation NavLocation;
				// Try to project the hit location onto the navigation mesh
				if (NavigationSystem->ProjectPointToNavigation(HitResult.Location, NavLocation, FVector(25.0f, 25.0f, 25.0f)))
				{
                    OutHitLocation = NavLocation.Location;
                    return true;  // Location is on the navmesh
                }
                else
                {
                    return false;  // Location is not on the navmesh
                }
			}
		}
	}
	return false;
}

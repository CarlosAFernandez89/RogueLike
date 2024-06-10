// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityTargetActor_MousePosition.h"

#include "NavigationSystem.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "ProjectEve/Character/EveCharacterHelper.h"
#include "ProjectEve/Player/EvePlayerController.h"

// Define the custom collision channel
#define COLLISION_GROUNDTARGETING ECC_GameTraceChannel1

FHitResult AGameplayAbilityTargetActor_MousePosition::PerformTrace(AActor* InSourceActor)
{
	bool bTraceComplex = false;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AGameplayAbilityTargetActor_GroundTrace), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActor(InSourceActor);

	FHitResult ReturnHitResult;

	if(AEvePlayerController* PC = IEveCharacterHelper::Execute_GetEvePlayerController(InSourceActor))
	{
		PC->GetHitResultUnderCursor(COLLISION_GROUNDTARGETING, bTraceComplex, ReturnHitResult);
		if (ReturnHitResult.bBlockingHit)  // Ensure there was a hit
		{
			// Get the navigation system and the current navigation data
			if (const UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
			{				
				FNavLocation NavLocation;
				// Try to project the hit location onto the navigation mesh
				if (NavigationSystem->ProjectPointToNavigation(ReturnHitResult.Location, NavLocation, FVector(25.0f, 25.0f, 25.0f)))
				{
					bLastTraceWasGood = true;  // Location is on the navmesh
				}
			}
		}
	}
	
	return ReturnHitResult;
}

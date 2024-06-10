// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityFunctionHelpers.h"


// Define the custom collision channel
#define COLLISION_GROUNDTARGETING ECC_GameTraceChannel1


bool UGameplayAbilityFunctionHelpers::GetHitLocationUnderCursor(FVector& OutHitLocation, const AActor* Target)
{
	if (const APlayerController* PC = Cast<APlayerController>(Target->GetInstigatorController()))
	{
		FHitResult HitResult;
		PC->GetHitResultUnderCursor(COLLISION_GROUNDTARGETING, false, HitResult);

		if (HitResult.bBlockingHit)  // Ensure there was a hit
		{
			OutHitLocation = HitResult.Location;
			return true;
		}
	}
	return false;
}

bool UGameplayAbilityFunctionHelpers::GetHitResultUnderCursor(FHitResult& OutHitLocation, const AActor* Target)
{
	if(const APlayerController* PC = Cast<APlayerController>(Target->GetInstigatorController()))
	{
		FHitResult HitResult;
		PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		if(HitResult.bBlockingHit)
		{
			OutHitLocation = HitResult;
			return true;
		}
	}
	return false;
}

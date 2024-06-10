// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_TargetLocation_Radius.h"


UGameplayAbility_TargetLocation_Radius::UGameplayAbility_TargetLocation_Radius()
{
	
}

bool UGameplayAbility_TargetLocation_Radius::GetHitLocationUnderCursor(FVector& OutHitLocation) const
{
	if (const APlayerController* PC = Cast<APlayerController>(GetAvatarActorFromActorInfo()->GetInstigatorController()))
	{
		FHitResult HitResult;
		PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		OutHitLocation = HitResult.Location;
		return true;
	}
	return false;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayAbilityFunctionHelpers.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTEVE_API UGameplayAbilityFunctionHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="GameplayAbilityFunctionHelpers")
	static bool GetHitLocationUnderCursor(FVector& OutHitLocation, const AActor* Target);

	UFUNCTION(BlueprintCallable, Category="GameplayAbilityFunctionHelpers")
	static bool GetHitResultUnderCursor(FHitResult& HitResult, const AActor* Target);
};

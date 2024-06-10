// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIPatrolRoute.generated.h"

UCLASS()
class PROJECTEVE_API AAIPatrolRoute : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default, meta = (AllowPrivateAccess = "true"))
	class USplineComponent* SplineComponent;

	UPROPERTY()
	int32 PatrolIndex;

	UPROPERTY()
	int32 PatrolDirection;

public:
	// Sets default values for this actor's properties
	AAIPatrolRoute();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable, Category="AIPatrolRoute")
	FVector GetSplinePointAsWorldPosition();

	UFUNCTION(BlueprintCallable, Category="AIPatrolRoute")
	void IncrementPatrolRoute();
};

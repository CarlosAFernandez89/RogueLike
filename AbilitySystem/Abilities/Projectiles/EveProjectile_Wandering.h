// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EveProjectile_Wandering.generated.h"

UCLASS()
class PROJECTEVE_API AEveProjectile_Wandering : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEveProjectile_Wandering();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Projectile movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	// Vector curve for the path
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
	class UCurveVector* PathCurve;

	// Sphere collider component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class USphereComponent* SphereCollider;
	
	// Interpolation speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
	float InterpSpeed;

	// Total duration of the movement before resetting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
	float TotalMovementTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	float Lifespan;

	// Method to update the projectile's position
	UFUNCTION(BlueprintNativeEvent, Category = "Movement")
	void UpdatePosition(float DeltaTime);

private:
	// Time to evaluate the curve
	float Time;

	// Initial location of the actor
	FVector InitialLocation;
	
};

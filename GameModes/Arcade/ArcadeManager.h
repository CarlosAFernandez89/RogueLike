// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArcadeManager.generated.h"

class UGameplayEffect;
class AEveCharacter;
class UInteractionComponent;

UCLASS()
class PROJECTEVE_API AArcadeManager : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Default, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* InteractionMeshComponent;
	
public:
	// Sets default values for this actor's properties
	AArcadeManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
	template <typename T>
	static void SpawnActorAsync(UWorld* World, TSubclassOf<T> ActorClass, const FTransform& SpawnTransform, TFunction<void(T*)> Callback);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Default, meta = (AllowPrivateAccess = "true"))
	UInteractionComponent* InteractionComponent;

	UFUNCTION()
	void OnInteract(AEveCharacter* InteractActor);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	class USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SplineRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	UDataTable* WaveDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
	TSubclassOf<UGameplayEffect> ScalingGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
	UCurveFloat* MaxHealthScalingCurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
	UCurveFloat* DamageScalingCurveFloat;
	
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void UpdateSplineRadius();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnActorsForCurrentWave(int32 CurrentWave, float& ProgressBarFillPerKill);

	UFUNCTION()
	void PrepareForNextWave();
};

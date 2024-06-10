// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EveGameMode.h"
#include "Arcade/ArcadeGameModeHelpers.h"
#include "EveArcadeGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveProgressUpdated, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveUpdated, int32, WaveNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllWavesCleared);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartNextWaveTimer, float, TimerLength);


USTRUCT(BlueprintType)
struct PROJECTEVE_API FWaveData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AActor>> ActorsToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfActorsToSpawn;
};


class AArcadeManager;
/**
 * 
 */
UCLASS()
class PROJECTEVE_API AEveArcadeGameMode : public AEveGameMode, public IArcadeGameModeHelpers
{
	GENERATED_BODY()

public:

	AEveArcadeGameMode();

	virtual void StartArcadeWave_Implementation() override;
	
	virtual void BeginPlay() override;

	void StartNextWave();

	UFUNCTION(BlueprintNativeEvent)
	void GiveWaveRewards();

	void OnMonsterKilled();

	UFUNCTION(BlueprintCallable)
	void RestartArcadeGameMode();
	
	UFUNCTION(BlueprintGetter)
	int32 GetCurrentStage() const { return CurrentStage; }

	UFUNCTION(BlueprintGetter)
	float GetWaveProgress() const { return ProgressBar; }

public:

	UPROPERTY()
	int32 NumberOfStages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
	float TimeBetweenStages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waves")
	bool bAutoStartNextWave;

	UPROPERTY()
	float ProgressBarFillPerKill;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnWaveUpdated OnWaveUpdated;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnWaveProgressUpdated OnWaveProgressUpdated;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAllWavesCleared OnAllWavesCleared;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStartNextWaveTimer OnStartNextWaveTimer;

private:
	
	int32 CurrentStage;
	int32 MonstersKilled;
	int32 TotalMonstersKilled;
	float ProgressBar;

	int GetNumberOfWaves() const;

	void SpawnMonsters();
	void ClearCurrentStage();

	UPROPERTY()
	AArcadeManager* ArcadeManager;
};

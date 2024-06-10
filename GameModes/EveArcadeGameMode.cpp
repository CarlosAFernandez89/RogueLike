// Fill out your copyright notice in the Description page of Project Settings.


#include "EveArcadeGameMode.h"

#include "Arcade/ArcadeManager.h"
#include "Kismet/GameplayStatics.h"

AEveArcadeGameMode::AEveArcadeGameMode(): NumberOfStages(0), TimeBetweenStages(3), bAutoStartNextWave(false),
                                          ProgressBarFillPerKill(0),
                                          CurrentStage(0),
                                          MonstersKilled(0),
                                          TotalMonstersKilled(0),
                                          ProgressBar(0),
                                          ArcadeManager(nullptr)
{
}

void AEveArcadeGameMode::StartArcadeWave_Implementation()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Starting EveArcadeGameMode."));
	}
	
	StartNextWave();
}

void AEveArcadeGameMode::BeginPlay()
{
	Super::BeginPlay();
	if(AActor* Manager = UGameplayStatics::GetActorOfClass(this, AArcadeManager::StaticClass()))
	{
		ArcadeManager = Cast<AArcadeManager>(Manager);
		NumberOfStages = GetNumberOfWaves();
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Failed to get AArcadeManager*. Please add an AArcadeManager to level."));
		}
	}
	
}


void AEveArcadeGameMode::StartNextWave()
{
	if (CurrentStage < NumberOfStages)
	{
		CurrentStage++;
		OnWaveUpdated.Broadcast(CurrentStage);
		SpawnMonsters();
	}
	else
	{
		// All stages completed, handle game end here
	}
}

void AEveArcadeGameMode::GiveWaveRewards_Implementation()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("Reward: All the monies")));
	}

	// GAME GETS PAUSED HERE IN BLUEPRINTS TO ALLOW PLAYER TO SELECT THE REWARDS... AFTER SELECTING REWARDS WE START THE TIMER.
	// Start the next wave after a delay
	FTimerHandle WaveDelay_TimerHandle;
	OnStartNextWaveTimer.Broadcast(TimeBetweenStages - 0.25f);
	GetWorldTimerManager().SetTimer(WaveDelay_TimerHandle, this, &AEveArcadeGameMode::StartNextWave, TimeBetweenStages, false);
		
}

void AEveArcadeGameMode::OnMonsterKilled()
{
	MonstersKilled++;
	TotalMonstersKilled++;
	ProgressBar += ProgressBarFillPerKill;
	OnWaveProgressUpdated.Broadcast(ProgressBar);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("Progress: %f"), ProgressBar));
	}
	
	if (ProgressBar >= 1.0f)
	{
		FTimerHandle ClearCurrentStage_TimerHandle;
		GetWorldTimerManager().SetTimer(ClearCurrentStage_TimerHandle, this, &AEveArcadeGameMode::ClearCurrentStage, 2.f, false);
	}
}

void AEveArcadeGameMode::RestartArcadeGameMode()
{
	CurrentStage = 1;
	TotalMonstersKilled = 0;
	MonstersKilled = 0;
	ProgressBar = 0.f;

	ArcadeManager->PrepareForNextWave();
}

int AEveArcadeGameMode::GetNumberOfWaves() const
{
	if(ArcadeManager->WaveDataTable)
	{
		TArray<FName> RowNames = ArcadeManager->WaveDataTable->GetRowNames();
		return RowNames.Num();
	}

	return 0;
}

void AEveArcadeGameMode::SpawnMonsters()
{
	// Implement your monster spawning logic here
	// You can use GetWorld()->SpawnActor<YourMonsterClass>(...) to spawn monsters

	ArcadeManager->SpawnActorsForCurrentWave(CurrentStage, ProgressBarFillPerKill);
	
}

void AEveArcadeGameMode::ClearCurrentStage()
{
	if(CurrentStage >= NumberOfStages)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("Cleared All Stages.")));
			OnAllWavesCleared.Broadcast();
		}
	}
	else
	{
		// Reset progress bar and kill count
		ProgressBar = 0.0f;
		MonstersKilled = 0;

		//Give wave rewards
		GiveWaveRewards();
	}

	if(!bAutoStartNextWave)
	{
		ArcadeManager->PrepareForNextWave();
	}

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "EvePlayerController.h"

#include "EngineUtils.h"
#include "ProjectEve/Equipment/EveQuickBarComponent.h"
#include "ProjectEve/Inventory/EveInventoryManagerComponent.h"

AEvePlayerController::AEvePlayerController()
{
	// Make the cursor always visible
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	
	InventoryManagerComponent = CreateDefaultSubobject<UEveInventoryManagerComponent>(TEXT("InventoryManagerComponent"));
	
	QuickBarComponent = CreateDefaultSubobject<UEveQuickBarComponent>(TEXT("QuickBarComponent"));
}

void AEvePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Set the input mode to Game and UI for mouse cursor visibility.
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
}

void AEvePlayerController::ServerCheatAll_Implementation(const FString& Msg)
{
	if (CheatManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerCheatAll: %s"), *Msg);
		for (TActorIterator<AEvePlayerController> It(GetWorld()); It; ++It)
		{
			AEvePlayerController* EvePC = (*It);
			if (EvePC)
			{
				EvePC->ClientMessage(EvePC->ConsoleCommand(Msg));
			}
		}
	}
}

bool AEvePlayerController::ServerCheatAll_Validate(const FString& Msg)
{
	return true;
}

void AEvePlayerController::ServerCheat_Implementation(const FString& Msg)
{
	if (CheatManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerCheat: %s"), *Msg);
		ClientMessage(ConsoleCommand(Msg));
	}
}

bool AEvePlayerController::ServerCheat_Validate(const FString& Msg)
{
	return true;
}

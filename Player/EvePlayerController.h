// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularPlayerController.h"
#include "EvePlayerController.generated.h"

class UEveQuickBarComponent;
class UEveInventoryManagerComponent;
class UEveEquipmentManagerComponent;
/**
 * 
 */
UCLASS()
class PROJECTEVE_API AEvePlayerController : public AGSCModularPlayerController
{
	GENERATED_BODY()

public:
	AEvePlayerController();

	virtual void BeginPlay() override;

	// Run a cheat command on the server.
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCheat(const FString& Msg);

	// Run a cheat command on the server for all players.
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCheatAll(const FString& Msg);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UEveInventoryManagerComponent* InventoryManagerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	UEveQuickBarComponent* QuickBarComponent;

};

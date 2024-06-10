// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "EveCheatManager.generated.h"

class UGameplayEffect;
class UGSCAbilitySystemComponent;
/**
 * 
 */
UCLASS(config = Game, Within = PlayerController, MinimalAPI)
class UEveCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:

	UEveCheatManager();

	static TSubclassOf<UGameplayEffect> LoadGameplayEffectFromClass(const FString& Path);

	virtual void InitCheatManager() override;

	// Helper function to write text to the console and to the log.
	static void CheatOutputText(const FString& TextToOutput);

	// Runs a cheat on the server for the owning player.
	UFUNCTION(exec)
	void Cheat(const FString& Msg);

	// Runs a cheat on the server for the all players.
	UFUNCTION(exec)
	void CheatAll(const FString& Msg);

	UFUNCTION(Exec)
	virtual void CycleAbilitySystemDebug();
	

	// Applies the specified damage amount to the owning player.
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	virtual void DamageSelf(float DamageAmount);

	// Applies the specified damage amount to the actor that the player is looking at.
	virtual void DamageTarget(float DamageAmount) override;

	// Applies the specified amount of healing to the owning player.
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	virtual void HealSelf(float HealAmount);

	// Applies the specified amount of healing to the actor that the player is looking at.
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	virtual void HealTarget(float HealAmount);

	// Applies enough damage to kill the owning player.
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	virtual void DamageSelfDestruct();

	// Prevents the owning player from taking any damage.
	virtual void God() override;

	// Prevents the owning player from dropping below 1 health.
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	virtual void UnlimitedHealth(int32 Enabled = -1);

	// Prints all items in the inventory
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	virtual void PrintAllInventoryItems();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffects")
	TSubclassOf<UGameplayEffect> HealGE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffects")
	TSubclassOf<UGameplayEffect> DamageGE;
	
protected:
	

	void ApplySetByCallerDamage(UGSCAbilitySystemComponent* EveASC, float DamageAmount);
	void ApplySetByCallerHeal(UGSCAbilitySystemComponent* EveASC, float HealAmount);

	UGSCAbilitySystemComponent* GetPlayerAbilitySystemComponent() const;



};

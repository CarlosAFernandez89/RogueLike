// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEve/Equipment/EveEquipmentManagerComponent.h"
#include "UObject/Interface.h"
#include "EveCharacterHelper.generated.h"

class AEvePlayerController;
class UEveEquipmentManagerComponent;
class UEveInventoryManagerComponent;
class UEveQuickBarComponent;
// This class does not need to be modified.
UINTERFACE()
class UEveCharacterHelper : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTEVE_API IEveCharacterHelper
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EveCharacterHelper")
	FVector GetLookAtDirection();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EveCharacterHelper")
	void StopMovementImmediately();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EveCharacterHelper")
	bool IsDead();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EveCharacterHelper|Interaction")
	void OnClickedActor(AActor* TouchedActor, FKey PressedKey);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="EveCharacterHelper|Interaction")
	void ClearInteractionDelegate();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="EveCharacterHelper|Equipment")
	UEveQuickBarComponent* GetQuickBarComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="EveCharacterHelper|Equipment")
	UEveInventoryManagerComponent* GetInventoryManagerComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="EveCharacterHelper|Equipment")
	FEveEquipmentList GetEquipmentList();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="EveCharacterHelper|Equipment")
	void SetEquippedItem(UEveEquipmentInstance* EquipmentInstance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="EveCharacterHelper|Equipment")
	UEveEquipmentInstance* GetEquippedItem();

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GameplayTagStack")
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GameplayTagStack")
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GameplayTagStack")
	int32 GetStatTagStackCount(FGameplayTag Tag);

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GameplayTagStack")
	bool HasStatTag(FGameplayTag Tag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GameplayAbility")
	void OnNormalDamageDone(const float DamageDone);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GameplayAbility")
	void OnCriticalDamageDone(const float DamageDone);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GameplayAbility")
	void OnDamageTaken(const float DamageTaken, const AActor* InstigatorActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="PlayerController")
	AEvePlayerController* GetEvePlayerController();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "UObject/Class.h"
#include "AbilityPowerUps.generated.h"

// Forward declarations
class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EPowerUpRarity : uint8
{
	Basic     UMETA(DisplayName = "Basic"),
	Rare      UMETA(DisplayName = "Rare"),
	Epic      UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary"),
	None      UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class EPowerUpWeaponType : uint8
{
	Generic    UMETA(DisplayName = "Generic"),
	Staff      UMETA(DisplayName = "Staff"),
	Bow        UMETA(DisplayName = "Bow"),
	GreatSword UMETA(DisplayName = "GreatSword"),
	None       UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct PROJECTEVE_API FPowerUpTagMagnitude
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag GameplayTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Magnitude = 0.f;
};

USTRUCT(BlueprintType)
struct PROJECTEVE_API FPowerUpTagQuantity
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag GameplayTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 0;
};

USTRUCT(BlueprintType)
struct PROJECTEVE_API FPowerUpGameplayAbility : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPowerUpRarity Rarity = EPowerUpRarity::Basic;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPowerUpWeaponType WeaponType = EPowerUpWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AbilityIcon = nullptr;
};

USTRUCT(BlueprintType)
struct PROJECTEVE_API FPowerUpGameplayEffect : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPowerUpRarity Rarity = EPowerUpRarity::Basic;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPowerUpWeaponType WeaponType = EPowerUpWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPowerUpTagMagnitude> TagMagnitudes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* EffectIcon = nullptr;
};

USTRUCT(BlueprintType)
struct PROJECTEVE_API FPowerUpGameplayTag : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPowerUpRarity Rarity = EPowerUpRarity::Basic;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPowerUpWeaponType WeaponType = EPowerUpWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPowerUpTagQuantity> GameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* EffectIcon = nullptr;
};

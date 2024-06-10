// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_PowerUpRewardsBase.h"

#include "GameplayEffect.h"
#include "Widget_PowerUpSlotBase.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectEve/AbilitySystem/Abilities/AbilityPowerUps.h"
#include "ProjectEve/GameModes/EveArcadeGameMode.h"


UWidget_PowerUpRewardsBase::UWidget_PowerUpRewardsBase(): CurrentStage(0), SelectedPowerUpSlot(nullptr)
{
}

void UWidget_PowerUpRewardsBase::PopulatePowerUpSlotsArray(const TArray<UWidget_PowerUpSlotBase*>& InPowerUpSlots)
{
	for (UWidget_PowerUpSlotBase* Element : InPowerUpSlots)
	{
		if (Element)
		{
			Element->OnPowerUpSlotSelected.AddDynamic(this, &UWidget_PowerUpRewardsBase::OnSlotSelected);
			ChooseRewards(EPowerUpWeaponType::Staff, Element);
		}
	}
}

void UWidget_PowerUpRewardsBase::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(AEveArcadeGameMode* ArcadeGameMode = Cast<AEveArcadeGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		CurrentStage = ArcadeGameMode->GetCurrentStage();
	}

}

void UWidget_PowerUpRewardsBase::NativeOnActivated()
{
	Super::NativeOnActivated();
	
}

template <typename T>
T* UWidget_PowerUpRewardsBase::ChooseByRarity(const TArray<T*>& FilteredItems)
{
	// Define rarity chances (adjust values as needed)
	TMap<EPowerUpRarity, float> RarityChances = {
		{ EPowerUpRarity::Legendary, 0.05f },
		{ EPowerUpRarity::Epic, 0.15f },
		{ EPowerUpRarity::Rare, 0.3f },
		{ EPowerUpRarity::Basic, 0.5f }
	};

	// Collect items by rarity
	TMap<EPowerUpRarity, TArray<T*>> ItemsByRarity;
	for (T* Item : FilteredItems)
	{
		if (Item)
		{
			ItemsByRarity.FindOrAdd(Item->Rarity).Add(Item);
		}
	}

	// Roll for rarity and attempt to find an item
	float Roll = FMath::FRand();
	float CumulativeProbability = 0.0f;

	for (const auto& RarityPair : RarityChances)
	{
		CumulativeProbability += RarityPair.Value;

		if (Roll < CumulativeProbability)
		{
			if (ItemsByRarity.Contains(RarityPair.Key) && ItemsByRarity[RarityPair.Key].Num() > 0)
			{
				const TArray<T*>& Items = ItemsByRarity[RarityPair.Key];
				return Items[FMath::RandRange(0, Items.Num() - 1)];
			}
		}
	}

	// If no items found for rolled rarity, fall back to lower rarities
	for (EPowerUpRarity Rarity : {EPowerUpRarity::Legendary, EPowerUpRarity::Epic, EPowerUpRarity::Rare, EPowerUpRarity::Basic})
	{
		if (ItemsByRarity.Contains(Rarity) && ItemsByRarity[Rarity].Num() > 0)
		{
			const TArray<T*>& Items = ItemsByRarity[Rarity];
			return Items[FMath::RandRange(0, Items.Num() - 1)];
		}
	}

	// If no items found by rarity, pick a random item from the filtered list
	if (FilteredItems.Num() > 0)
	{
		return FilteredItems[FMath::RandRange(0, FilteredItems.Num() - 1)];
	}

	return nullptr;  // Should never reach here since we always have a fallback
}

void UWidget_PowerUpRewardsBase::ChooseRewards(const EPowerUpWeaponType DesiredType, UWidget_PowerUpSlotBase* PowerUpSlot)
{
    if (RewardsDataTables.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No data tables available."));
        return;
    }

    // Choose a random data table
	const int32 ChosenDataTable = FMath::RandRange(0, RewardsDataTables.Num() - 1);
    UDataTable* SelectedDataTable = RewardsDataTables[ChosenDataTable];
    if (!SelectedDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("Selected data table is null."));
        return;
    }

    // Filter the rows based on the desired type
    TArray<FPowerUpGameplayAbility*> FilteredAbilities;
    TArray<FPowerUpGameplayEffect*> FilteredEffects;
    TArray<FPowerUpGameplayTag*> FilteredTags;



    for (auto& Row : SelectedDataTable->GetRowMap())
    {
        // Check the row type and filter accordingly
        if (FPowerUpGameplayAbility* Ability = reinterpret_cast<FPowerUpGameplayAbility*>(Row.Value); ChosenDataTable == 0)
        {
            if (Ability->WeaponType == EPowerUpWeaponType::Generic || Ability->WeaponType == DesiredType)
            {
                FilteredAbilities.Add(Ability);
            }
        }
        else if (FPowerUpGameplayEffect* Effect = reinterpret_cast<FPowerUpGameplayEffect*>(Row.Value); ChosenDataTable == 1)
        {
            if (Effect->WeaponType == EPowerUpWeaponType::Generic || Effect->WeaponType == DesiredType)
            {
                FilteredEffects.Add(Effect);
            }
        }
        else if (FPowerUpGameplayTag* Tag = reinterpret_cast<FPowerUpGameplayTag*>(Row.Value); ChosenDataTable == 2)
        {
            if (Tag->WeaponType == EPowerUpWeaponType::Generic || Tag->WeaponType == DesiredType)
            {
                FilteredTags.Add(Tag);
            }
        }
    }

	// Choose an item based on rarity from the filtered results
	if (FilteredAbilities.Num() > 0 && ChosenDataTable == 0)
	{
		if (FPowerUpGameplayAbility* SelectedAbility = ChooseByRarity(FilteredAbilities))
		{
			PowerUpSlot->SetupSlot(*SelectedAbility);
		}
	}
	else if (FilteredEffects.Num() > 0 && ChosenDataTable == 1)
	{
		if (FPowerUpGameplayEffect* SelectedEffect = ChooseByRarity(FilteredEffects))
		{
			PowerUpSlot->SetupSlot(FPowerUpGameplayAbility(), *SelectedEffect);
		}
	}
	else if (FilteredTags.Num() > 0 && ChosenDataTable == 2)
	{
		if (FPowerUpGameplayTag* SelectedTag = ChooseByRarity(FilteredTags))
		{
			PowerUpSlot->SetupSlot(FPowerUpGameplayAbility(), FPowerUpGameplayEffect(), *SelectedTag);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No matching items found."));
	}

	FilteredAbilities.Empty();
	FilteredEffects.Empty();
	FilteredTags.Empty();
}

void UWidget_PowerUpRewardsBase::OnSlotSelected_Implementation(UWidget_PowerUpSlotBase* SlotReference)
{
	if (SelectedPowerUpSlot)
	{
		SelectedPowerUpSlot->DeSelectSlot();
	}

	SelectedPowerUpSlot = SlotReference;

	if (SelectedPowerUpSlot)
	{
		SelectedPowerUpSlot->SelectSlot();
	}
}



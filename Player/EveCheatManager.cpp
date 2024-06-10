// Fill out your copyright notice in the Description page of Project Settings.


#include "EveCheatManager.h"

#include "AbilitySystemGlobals.h"
#include "EvePlayerController.h"
#include "EvePlayerState.h"
#include "Abilities/GSCAbilitySystemComponent.h"
#include "Components/GSCCoreComponent.h"
#include "Engine/Console.h"
#include "GameFramework/HUD.h"
#include "ProjectEve/Character/EveCharacter.h"
#include "ProjectEve/Inventory/EveInventoryItemDefinition.h"
#include "ProjectEve/Inventory/EveInventoryItemInstance.h"
#include "ProjectEve/Inventory/EveInventoryManagerComponent.h"

namespace EveCheat
{
	static const FName NAME_Fixed = FName(TEXT("Fixed"));
	
	static bool bEnableDebugCameraCycling = false;
	static FAutoConsoleVariableRef CVarEnableDebugCameraCycling(
		TEXT("EveCheat.EnableDebugCameraCycling"),
		bEnableDebugCameraCycling,
		TEXT("If true then you can cycle the debug camera while running the game."),
		ECVF_Cheat);

	static bool bStartInGodMode = false;
	static FAutoConsoleVariableRef CVarStartInGodMode(
		TEXT("EveCheat.StartInGodMode"),
		bStartInGodMode,
		TEXT("If true then the God cheat will be applied on begin play"),
		ECVF_Cheat);
};

UEveCheatManager::UEveCheatManager()
{
}

TSubclassOf<UGameplayEffect> UEveCheatManager::LoadGameplayEffectFromClass(const FString& Path)
{
	// Notice the proper Unreal path format below
	UClass* LoadedClass = LoadClass<UGameplayEffect>(nullptr, *Path);
	if (LoadedClass != nullptr)
	{
		return LoadedClass;
	}
	return nullptr; // or handle the error as appropriate
}

void UEveCheatManager::InitCheatManager()
{
	Super::InitCheatManager();
	
	if (EveCheat::bStartInGodMode)
	{
		God();	
	}
}

void UEveCheatManager::CheatOutputText(const FString& TextToOutput)
{
	// Output to the console.
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->ViewportConsole)
	{
		GEngine->GameViewport->ViewportConsole->OutputText(TextToOutput);
	}

	// Output to log.
	UE_LOG(LogTemp, Display, TEXT("%s"), *TextToOutput);
}

void UEveCheatManager::Cheat(const FString& Msg)
{
	if (AEvePlayerController* EvePC = Cast<AEvePlayerController>(GetOuterAPlayerController()))
	{
		EvePC->ServerCheat(Msg.Left(128));
	}
}

void UEveCheatManager::CheatAll(const FString& Msg)
{
	if (AEvePlayerController* EvePC = Cast<AEvePlayerController>(GetOuterAPlayerController()))
	{
		EvePC->ServerCheatAll(Msg.Left(128));
	}
}

void UEveCheatManager::CycleAbilitySystemDebug()
{
	APlayerController* PC = Cast<APlayerController>(GetOuterAPlayerController());

	if (PC && PC->MyHUD)
	{
		if (!PC->MyHUD->bShowDebugInfo || !PC->MyHUD->DebugDisplay.Contains(TEXT("AbilitySystem")))
		{
			PC->MyHUD->ShowDebug(TEXT("AbilitySystem"));
		}

		PC->ConsoleCommand(TEXT("AbilitySystem.Debug.NextCategory"));
	}
}

void UEveCheatManager::DamageSelf(float DamageAmount)
{
	if (UGSCAbilitySystemComponent* EveASC = GetPlayerAbilitySystemComponent())
	{
		ApplySetByCallerDamage(EveASC, DamageAmount);
	}
}

void UEveCheatManager::DamageTarget(float DamageAmount)
{
	if (AEvePlayerController* EvePC = Cast<AEvePlayerController>(GetOuterAPlayerController()))
	{
		if (EvePC->GetNetMode() == NM_Client)
		{
			// Automatically send cheat to server for convenience.
			EvePC->ServerCheat(FString::Printf(TEXT("DamageTarget %.2f"), DamageAmount));
			return;
		}

		FHitResult TargetHitResult;
		AActor* TargetActor = GetTarget(EvePC, TargetHitResult);

		if (UGSCAbilitySystemComponent* EveTargetASC = Cast<UGSCAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor)))
		{
			ApplySetByCallerDamage(EveTargetASC, DamageAmount);
		}
	}
}

void UEveCheatManager::HealSelf(float HealAmount)
{
	if (UGSCAbilitySystemComponent* EveASC = GetPlayerAbilitySystemComponent())
	{
		ApplySetByCallerHeal(EveASC, HealAmount);
	}
}

void UEveCheatManager::HealTarget(float HealAmount)
{
	if (AEvePlayerController* EvePC = Cast<AEvePlayerController>(GetOuterAPlayerController()))
	{
		FHitResult TargetHitResult;
		AActor* TargetActor = GetTarget(EvePC, TargetHitResult);

		if (UGSCAbilitySystemComponent* EveTargetASC = Cast<UGSCAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor)))
		{
			ApplySetByCallerHeal(EveTargetASC, HealAmount);
		}
	}
}

void UEveCheatManager::DamageSelfDestruct()
{
	if (AEvePlayerController* EvePC = Cast<AEvePlayerController>(GetOuterAPlayerController()))
	{
		if(AEveCharacter* EveCharacter = Cast<AEveCharacter>(EvePC->GetPawn()))
		{
			if (UGSCAbilitySystemComponent* EveASC = GetPlayerAbilitySystemComponent())
			{
				ApplySetByCallerDamage(EveASC, EveCharacter->GSCCoreComponent->GetMaxHealth());
			}
		}
	}
}

void UEveCheatManager::God()
{
	if (AEvePlayerController* EvePC = Cast<AEvePlayerController>(GetOuterAPlayerController()))
	{
		if (EvePC->GetNetMode() == NM_Client)
		{
			// Automatically send cheat to server for convenience.
			EvePC->ServerCheat(FString::Printf(TEXT("God")));
			return;
		}

		if (UGSCAbilitySystemComponent* EveASC = GetPlayerAbilitySystemComponent())
		{
			const FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Cheat.God");

			if (const bool bHasTag = EveASC->HasMatchingGameplayTag(Tag))
			{
				EveASC->RemoveLooseGameplayTag(Tag);
			}
			else
			{
				EveASC->AddLooseGameplayTag(Tag);
			}
		}
	}
}

void UEveCheatManager::UnlimitedHealth(int32 Enabled)
{
	if (UGSCAbilitySystemComponent* EveASC = GetPlayerAbilitySystemComponent())
	{
		const FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Cheat.UnlimitedHealth");
		const bool bHasTag = EveASC->HasMatchingGameplayTag(Tag);

		if ((Enabled == -1) || ((Enabled > 0) && !bHasTag) || ((Enabled == 0) && bHasTag))
		{
			if (bHasTag)
			{
				EveASC->RemoveLooseGameplayTag(Tag);
			}
			else
			{
				EveASC->AddLooseGameplayTag(Tag);
			}
		}
	}
}

void UEveCheatManager::PrintAllInventoryItems()
{
	if (AEvePlayerController* EvePC = Cast<AEvePlayerController>(GetOuterAPlayerController()))
	{
		if(UEveInventoryManagerComponent* InventoryManagerComponent = EvePC->InventoryManagerComponent)
		{

			for (auto Element : InventoryManagerComponent->GetAllItems())
			{
				FString FormattedString = FString::Printf(TEXT("%s"), *Element->GetItemDef()->GetName());
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FormattedString);
				}
			}

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Items in Inventory: ")));
			}
		}
	}
}

void UEveCheatManager::ApplySetByCallerDamage(UGSCAbilitySystemComponent* EveASC, float DamageAmount)
{
	check(EveASC);
	if(DamageGE)
	{
		FGameplayEffectSpecHandle SpecHandle = EveASC->MakeOutgoingSpec(DamageGE, 1.0f, EveASC->MakeEffectContext());

		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("SetByCaller.Damage"), DamageAmount);
			EveASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void UEveCheatManager::ApplySetByCallerHeal(UGSCAbilitySystemComponent* EveASC, float HealAmount)
{
	check(EveASC);

	if(HealGE)
	{
		FGameplayEffectSpecHandle SpecHandle = EveASC->MakeOutgoingSpec(HealGE, 1.0f, EveASC->MakeEffectContext());

		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("SetByCaller.Heal"), HealAmount);
			EveASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

UGSCAbilitySystemComponent* UEveCheatManager::GetPlayerAbilitySystemComponent() const
{
	if (AEvePlayerController* EvePC = Cast<AEvePlayerController>(GetOuterAPlayerController()))
	{
		return Cast<UGSCAbilitySystemComponent>(EvePC->GetPlayerState<AEvePlayerState>()->GetAbilitySystemComponent());
	}
	return nullptr;
}

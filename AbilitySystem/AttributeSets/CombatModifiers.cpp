// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatModifiers.h"

#include "Net/UnrealNetwork.h"

// Sets default values
UCombatModifiers::UCombatModifiers()
{
	// Default constructor
}

void UCombatModifiers::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    // This is called whenever attributes change, so for max attributes we want to scale the current totals to match
    Super::PreAttributeChange(Attribute, NewValue);

    // Set adjust code here
    //
    // Example:
    //
    // If a Max value changes, adjust current to keep Current % of Current to Max
    //
    // if (Attribute == GetMaxHealthAttribute())
    // {
    //     AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
    // }

    if(Attribute == GetCriticalStrikeChanceAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f,1.f);
    }
    else if (Attribute == GetDoubleDamageChanceAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f,1.f);
    }
    else if(Attribute == GetAttackSpeedAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.1f, 3.f);
    }
    else if (Attribute == GetDamageLeechAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f,1.f);
    }
    else if (Attribute == GetManaLeechAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f,1.f);
    }
    else if(Attribute == GetDamageReductionAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f,0.75f);
    }
    
}

void UCombatModifiers::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    FGSCAttributeSetExecutionData ExecutionData;
    GetExecutionDataFromMod(Data, ExecutionData);

    // Set clamping or handling or "meta" attributes here (like damages)

    // Example 1: Using helpers to handle each attribute in their own methods (See GSCAttributeSet.cpp)

    // if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    // {
    //     HandleHealthAttribute(ExecutionData);
    // }

    // Example 2: Basic example to clamp the value of an Health Attribute between 0 and another MaxHealth Attribute

    // if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    // {
    //     SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    // }
}

void UCombatModifiers::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
    DOREPLIFETIME_CONDITION_NOTIFY(UCombatModifiers, ExtraDamageAdditive, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UCombatModifiers, ExtraDamageMultiplier, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UCombatModifiers, DoubleDamageChance, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UCombatModifiers, AttackSpeed, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UCombatModifiers, CriticalStrikeChance, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UCombatModifiers, CriticalStrikeDamageMultiplier, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UCombatModifiers, DamageLeech, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UCombatModifiers, ManaLeech, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UCombatModifiers, DamageReduction, COND_None, REPNOTIFY_Always);
}

void UCombatModifiers::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    Super::PreAttributeBaseChange(Attribute, NewValue);

    if(Attribute == GetCriticalStrikeChanceAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f,1.f);
    }
    else if (Attribute == GetDoubleDamageChanceAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f,1.f);
    }
    else if(Attribute == GetAttackSpeedAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.1f, 3.f);
    }
    else if (Attribute == GetDamageLeechAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f,1.f);
    }
    else if (Attribute == GetManaLeechAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f,1.f);
    }
    else if(Attribute == GetDamageReductionAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f,0.75f);
    }
}

void UCombatModifiers::OnRep_ExtraDamageAdditive(const FGameplayAttributeData& OldExtraDamageAdditive)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatModifiers, ExtraDamageAdditive, OldExtraDamageAdditive);
}

void UCombatModifiers::OnRep_ExtraDamageMultiplier(const FGameplayAttributeData& OldExtraDamageMultiplier)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatModifiers, ExtraDamageMultiplier, OldExtraDamageMultiplier);
}

void UCombatModifiers::OnRep_DoubleDamageChance(const FGameplayAttributeData& OldDoubleDamageChance)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatModifiers, DoubleDamageChance, OldDoubleDamageChance);
}

void UCombatModifiers::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatModifiers, AttackSpeed, OldAttackSpeed);
}

void UCombatModifiers::OnRep_CriticalStrikeChance(const FGameplayAttributeData& OldCriticalStrikeChance)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatModifiers, CriticalStrikeChance, OldCriticalStrikeChance);
}

void UCombatModifiers::OnRep_CriticalStrikeDamageMultiplier(const FGameplayAttributeData& OldCriticalStrikeDamageMultiplier)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatModifiers, CriticalStrikeDamageMultiplier, OldCriticalStrikeDamageMultiplier);
}

void UCombatModifiers::OnRep_DamageLeech(const FGameplayAttributeData& OldDamageLeech)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatModifiers, DamageLeech, OldDamageLeech);
}

void UCombatModifiers::OnRep_ManaLeech(const FGameplayAttributeData& OldManaLeech)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatModifiers, ManaLeech, OldManaLeech);
}

void UCombatModifiers::OnRep_DamageReduction(const FGameplayAttributeData& OldDamageReduction)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatModifiers, DamageReduction, OldDamageReduction);
}

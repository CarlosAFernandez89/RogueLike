// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Attributes/GSCAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "CombatModifiers.generated.h"

UCLASS()
class PROJECTEVE_API UCombatModifiers : public UGSCAttributeSetBase
{
	GENERATED_BODY()

public:

	// Sets default values for this AttributeSet attributes
	UCombatModifiers();

    // AttributeSet Overrides
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
        
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_ExtraDamageAdditive)
    FGameplayAttributeData ExtraDamageAdditive = 0.0;
    ATTRIBUTE_ACCESSORS(UCombatModifiers, ExtraDamageAdditive)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_ExtraDamageMultiplier)
    FGameplayAttributeData ExtraDamageMultiplier = 0.0;
    ATTRIBUTE_ACCESSORS(UCombatModifiers, ExtraDamageMultiplier)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_DoubleDamageChance)
    FGameplayAttributeData DoubleDamageChance = 0.0;
    ATTRIBUTE_ACCESSORS(UCombatModifiers, DoubleDamageChance)

	UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_AttackSpeed)
	FGameplayAttributeData AttackSpeed = 0.3;
	ATTRIBUTE_ACCESSORS(UCombatModifiers, AttackSpeed)  
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_CriticalStrikeChance)
    FGameplayAttributeData CriticalStrikeChance = 0.3;
    ATTRIBUTE_ACCESSORS(UCombatModifiers, CriticalStrikeChance)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_CriticalStrikeDamageMultiplier)
    FGameplayAttributeData CriticalStrikeDamageMultiplier = 1.2;
    ATTRIBUTE_ACCESSORS(UCombatModifiers, CriticalStrikeDamageMultiplier)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_DamageLeech)
    FGameplayAttributeData DamageLeech = 0.0;
    ATTRIBUTE_ACCESSORS(UCombatModifiers, DamageLeech)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_ManaLeech)
    FGameplayAttributeData ManaLeech = 0.0;
    ATTRIBUTE_ACCESSORS(UCombatModifiers, ManaLeech)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_DamageReduction)
    FGameplayAttributeData DamageReduction = 0.0;
    ATTRIBUTE_ACCESSORS(UCombatModifiers, DamageReduction)    

protected:
    
    UFUNCTION()
    virtual void OnRep_ExtraDamageAdditive(const FGameplayAttributeData& OldExtraDamageAdditive);

    UFUNCTION()
    virtual void OnRep_ExtraDamageMultiplier(const FGameplayAttributeData& OldExtraDamageMultiplier);

    UFUNCTION()
    virtual void OnRep_DoubleDamageChance(const FGameplayAttributeData& OldDoubleDamageChance);

	UFUNCTION()
	virtual void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed);

    UFUNCTION()
    virtual void OnRep_CriticalStrikeChance(const FGameplayAttributeData& OldCriticalStrikeChance);

    UFUNCTION()
    virtual void OnRep_CriticalStrikeDamageMultiplier(const FGameplayAttributeData& OldCriticalStrikeDamageMultiplier);

    UFUNCTION()
    virtual void OnRep_DamageLeech(const FGameplayAttributeData& OldDamageLeech);

    UFUNCTION()
    virtual void OnRep_ManaLeech(const FGameplayAttributeData& OldManaLeech);

    UFUNCTION()
    virtual void OnRep_DamageReduction(const FGameplayAttributeData& OldDamageReduction);
	
	
};

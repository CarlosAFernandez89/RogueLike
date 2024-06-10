// Fill out your copyright notice in the Description page of Project Settings.


#include "EveGameplayAbility_FromEquipment.h"

#include "EveEquipmentInstance.h"
#include "ProjectEve/Character/EveCharacterHelper.h"
#include "ProjectEve/Inventory/EveInventoryItemInstance.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UEveGameplayAbility_FromEquipment::UEveGameplayAbility_FromEquipment()
{
}

UEveEquipmentInstance* UEveGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if(UEveEquipmentInstance* EquipmentInstance = IEveCharacterHelper::Execute_GetEquippedItem(GetOwningActorFromActorInfo()))
	{
		return EquipmentInstance;
	}
	
	return nullptr;
}

UEveInventoryItemInstance* UEveGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UEveEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UEveInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}


#if WITH_EDITOR
EDataValidationResult UEveGameplayAbility_FromEquipment::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		Context.AddError(NSLOCTEXT("Eve", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}
#endif
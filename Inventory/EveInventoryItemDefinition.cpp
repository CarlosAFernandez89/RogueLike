// Fill out your copyright notice in the Description page of Project Settings.


#include "EveInventoryItemDefinition.h"

UEveInventoryItemDefinition::UEveInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UEveInventoryItemFragment* UEveInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<UEveInventoryItemFragment> FragmentClass) const
{
	if(FragmentClass != nullptr)
	{
		for(UEveInventoryItemFragment* Fragment : Fragments)
		{
			if(Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

const UEveInventoryItemFragment* UEveInventoryFunctionLibrary::FindItemDefinitionFragment(
	TSubclassOf<UEveInventoryItemDefinition> ItemDef, TSubclassOf<UEveInventoryItemFragment> FragmentClass)
{
	if((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UEveInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	
	return nullptr;
}

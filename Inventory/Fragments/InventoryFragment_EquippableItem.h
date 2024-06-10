// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "ProjectEve/Inventory/EveInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"

#include "InventoryFragment_EquippableItem.generated.h"

class UEveEquipmentDefinition;
class UObject;

UCLASS()
class UInventoryFragment_EquippableItem : public UEveInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Eve)
	TSubclassOf<UEveEquipmentDefinition> EquipmentDefinition;
};

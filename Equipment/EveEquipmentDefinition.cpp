// Fill out your copyright notice in the Description page of Project Settings.


#include "EveEquipmentDefinition.h"

#include "EveEquipmentInstance.h"

UEveEquipmentDefinition::UEveEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UEveEquipmentInstance::StaticClass();
}

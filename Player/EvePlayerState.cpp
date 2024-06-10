// Fill out your copyright notice in the Description page of Project Settings.


#include "EvePlayerState.h"

UEveEquipmentInstance* AEvePlayerState::GetEquippedItem_Implementation()
{
	return IEveCharacterHelper::Execute_GetEquippedItem(GetOwningController()->GetPawn());
}

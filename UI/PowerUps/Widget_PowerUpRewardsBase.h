// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"

#include "Widget_PowerUpRewardsBase.generated.h"

enum class EPowerUpWeaponType : uint8;
class UWidget_PowerUpSlotBase;
/**
 * 
 */
UCLASS()
class PROJECTEVE_API UWidget_PowerUpRewardsBase : public UCommonActivatableWidget
{
	GENERATED_BODY()


public:

	UWidget_PowerUpRewardsBase();

	UFUNCTION(BlueprintCallable)
	void PopulatePowerUpSlotsArray(const TArray<UWidget_PowerUpSlotBase*>& InPowerUpSlots);

	UFUNCTION(BlueprintNativeEvent)
	void OnSlotSelected(UWidget_PowerUpSlotBase* SlotReference);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;

	template<typename T>
	T* ChooseByRarity(const TArray<T*>& FilteredItems);

private:

	void ChooseRewards(const EPowerUpWeaponType DesiredType, UWidget_PowerUpSlotBase* PowerUpSlot);

protected:

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentStage;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DataTables")
	TArray<UDataTable*> RewardsDataTables;
	

	UPROPERTY(BlueprintReadWrite)
	UWidget_PowerUpSlotBase* SelectedPowerUpSlot;
	
};



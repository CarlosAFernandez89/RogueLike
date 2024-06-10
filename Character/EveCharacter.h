// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EveCharacterHelper.h"
#include "ModularGameplayActors/GSCModularPlayerStateCharacter.h"
#include "ProjectEve/System/GameplayTagStack.h"

#include "EveCharacter.generated.h"

class UEveInventoryManagerComponent;
class UEveEquipmentManagerComponent;
class UGSCCoreComponent;
class UGSCAbilityInputBindingComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECTEVE_API AEveCharacter : public AGSCModularPlayerStateCharacter, public IEveCharacterHelper
{
	GENERATED_BODY()

	UFUNCTION()
	void DistanceCheckForInteract(const AActor* TouchedActor);
	UPROPERTY()
	FTimerHandle DistanceForInteract_TimerHandle;
	
public:
	// Sets default values for this character's properties
	AEveCharacter(const FObjectInitializer& ObjectInitializer);

	// IEveCharacterHelpers
	virtual FVector GetLookAtDirection_Implementation() override;
	virtual void StopMovementImmediately_Implementation() override;
	virtual bool IsDead_Implementation() override;
	virtual void OnClickedActor_Implementation(AActor* TouchedActor, FKey PressedKey) override;
	virtual void ClearInteractionDelegate_Implementation() override;
	virtual UEveQuickBarComponent* GetQuickBarComponent_Implementation() override;
	virtual UEveInventoryManagerComponent* GetInventoryManagerComponent_Implementation() override;
	virtual FEveEquipmentList GetEquipmentList_Implementation() override;
	virtual void SetEquippedItem_Implementation(UEveEquipmentInstance* EquipmentInstance) override;
	virtual UEveEquipmentInstance* GetEquippedItem_Implementation() override;
	virtual void AddStatTagStack_Implementation(FGameplayTag Tag, int32 StackCount) override;
	virtual void RemoveStatTagStack_Implementation(FGameplayTag Tag, int32 StackCount) override;
	virtual int32 GetStatTagStackCount_Implementation(FGameplayTag Tag) override;
	virtual bool HasStatTag_Implementation(FGameplayTag Tag) override;
	virtual void OnNormalDamageDone_Implementation(const float DamageDone) override;
	virtual void OnCriticalDamageDone_Implementation(const float DamageDone) override;
	virtual void OnDamageTaken_Implementation(const float DamageTaken, const AActor* InstigatorActor) override;
	virtual AEvePlayerController* GetEvePlayerController_Implementation() override;
	// End IEveCharacterHelpers

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	UCurveFloat* CameraZoomCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
	float CameraZoomFactor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction")
	float InteractionDistance;

private:

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void CameraZoom(float ZoomFactor);

	UFUNCTION()
	void CameraZoomMovement(const int32& TargetArmLength , const FRotator& Rotation);

	UPROPERTY()
	FRotator CameraBoomRotator_DefaultOffset;
	
	UPROPERTY()
	FTimerHandle CameraZoomTimerHandle;

	UPROPERTY()
	UEveEquipmentInstance* CurrentEquipmentInstance;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyRestarted() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UGSCAbilityInputBindingComponent* AbilityInputBindingComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UGSCCoreComponent* GSCCoreComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	UEveEquipmentManagerComponent* EquipmentManagerComponent;
	
};

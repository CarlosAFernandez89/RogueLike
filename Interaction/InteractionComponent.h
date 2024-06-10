// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractableTarget.h"
#include "Components/ActorComponent.h"
#include "InputCoreTypes.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClickedSignature, AActor*, TouchedActor, FKey, ButtonPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginCursorOverSignature, AActor*, TouchedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndCursorOverSignature, AActor*, TouchedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractWithActorSignature, AEveCharacter*, TouchedActor);



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTEVE_API UInteractionComponent : public UActorComponent, public IInteractableTarget
{
	GENERATED_BODY()

	

public:
	// Sets default values for this component's properties
	UInteractionComponent();

	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInterface* HoverOverMaterial;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaterialIndexToUse;


	UFUNCTION(Category="Interaction")
	void Interact_OnClicked(AActor* TouchedActor ,FKey ButtonPressed);
	
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnClickedSignature OnClicked;
	
	UFUNCTION(Category="Interaction")
	void Interact_OnBeginCursorOver(AActor* TouchedActor);

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnBeginCursorOverSignature OnBeginCursor;
	
	UFUNCTION(Category="Interaction")
	void Interact_OnEndCursorOver(AActor* TouchedActor);
	
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnEndCursorOverSignature OnEndCursorOver;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractWithActorSignature OnInteractWithActor; 
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// IInteractableTarget
	//virtual void CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag, FGameplayEventData& InOutEventData) override;
	virtual void InteractWithActor_Implementation(AEveCharacter* InteractActor) override;
	// ~IInteractableTarget



private:

	UPROPERTY()
	UMaterialInterface* BaseMaterial;

};

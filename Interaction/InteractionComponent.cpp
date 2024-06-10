// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectEve/Character/EveCharacterHelper.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent(): StaticMeshComponent(nullptr), HoverOverMaterial(nullptr),
                                                MaterialIndexToUse(0),
                                                BaseMaterial(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if(AActor* Owner = GetOwner())
	{
		Owner->OnClicked.AddDynamic(this, &UInteractionComponent::Interact_OnClicked);
		Owner->OnBeginCursorOver.AddDynamic(this, &UInteractionComponent::Interact_OnBeginCursorOver);
		Owner->OnEndCursorOver.AddDynamic(this, &UInteractionComponent::Interact_OnEndCursorOver);
		StaticMeshComponent = Owner->GetComponentByClass<UStaticMeshComponent>();
		BaseMaterial = StaticMeshComponent->GetMaterial(MaterialIndexToUse);
	}
}

void UInteractionComponent::InteractWithActor_Implementation(AEveCharacter* InteractActor)
{
	OnInteractWithActor.Broadcast(InteractActor);
}



void UInteractionComponent::Interact_OnBeginCursorOver(AActor* TouchedActor)
{
	if(HoverOverMaterial)
	{
		StaticMeshComponent->SetMaterial(MaterialIndexToUse, HoverOverMaterial);
	}
	
	StaticMeshComponent->SetRenderCustomDepth(true);

	OnBeginCursor.Broadcast(TouchedActor);
}

void UInteractionComponent::Interact_OnEndCursorOver(AActor* TouchedActor)
{
	if(HoverOverMaterial)
	{
		StaticMeshComponent->SetMaterial(MaterialIndexToUse, BaseMaterial);
	}
	
	StaticMeshComponent->SetRenderCustomDepth(false);

	OnEndCursorOver.Broadcast(TouchedActor);
}

void UInteractionComponent::Interact_OnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if(ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		IEveCharacterHelper::Execute_OnClickedActor(Character ,TouchedActor, ButtonPressed);
	}
	
	OnClicked.Broadcast(TouchedActor, ButtonPressed);
}



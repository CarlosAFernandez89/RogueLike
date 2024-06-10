// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "IInteractableTarget.generated.h"

class AEveCharacter;
/**  */
UINTERFACE(MinimalAPI)
class UInteractableTarget : public UInterface
{
	GENERATED_BODY()
};

/**  */
class PROJECTEVE_API IInteractableTarget
{
	GENERATED_BODY()

public:
	/**  */
	virtual void CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag, FGameplayEventData& InOutEventData) { }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractWithActor(AEveCharacter* InteractActor);
};

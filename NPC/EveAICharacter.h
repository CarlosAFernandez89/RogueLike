// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EveAIHelpers.h"
#include "ModularGameplayActors/GSCModularCharacter.h"
#include "EveAICharacter.generated.h"

class UBehaviorTree;
class AEveAIController;
enum class ECCCollisionPart : uint8;

UCLASS()
class PROJECTEVE_API AEveAICharacter : public AGSCModularCharacter, public IEveAIHelpers
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Default, meta = (AllowPrivateAccess = "true"))
	class UGSCCoreComponent* GSCCoreComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Default, meta = (AllowPrivateAccess = "true"))
	class UCCCollisionHandlerComponent* CCCollisionHandler;

	UPROPERTY()
	TMap<AActor*, int32> ReservedAttackTokens;

	UPROPERTY()
	AEveAIController* AIController;
	
public:
	// Sets default values for this character's properties
	AEveAICharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Patrol")
	AAIPatrolRoute* PatrolRoute = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	UBehaviorTree* BehaviorTree;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION()
	void CollisionHandler_OnCollisionActivated(ECCCollisionPart CollisionPart);
	UFUNCTION()
	void CollisionHandler_OnHit(const FHitResult& HitResult, UPrimitiveComponent* CollidingComponent);

	UFUNCTION(BlueprintNativeEvent)
	void OnDeath();



	//~ IAIHelpers
	virtual AAIPatrolRoute* GetAIPatrolRoute_Implementation() override;
	virtual float SetAIMovementSpeed_Implementation(EAIMovementSpeed MovementSpeed) override;
	virtual void GetAIIdealRange_Implementation(float& AttackRadius, float& DefendRadius) override;
	virtual float GetAICurrentHealth_Implementation() override;
	virtual float GetAIMaxHealth_Implementation() override;
	virtual bool GetAIIsDead_Implementation() override;
	virtual void AIStoreAttackTokens_Implementation(AActor* AttackTarget, int32 Amount) override;
	virtual void AIReturnAllAttackTokens_Implementation() override;
	virtual void NavLink_JumpToDestination_Implementation(FVector Destination) override;
	//~ End IAIHelpers

public:
	
	UFUNCTION(BlueprintCallable, Category="AI|GAS")
	UGSCCoreComponent* GetGSCCoreComponent() const
	{
		return GSCCoreComponent;
	}

	UFUNCTION(BlueprintNativeEvent)
	void DeathEvent();
};

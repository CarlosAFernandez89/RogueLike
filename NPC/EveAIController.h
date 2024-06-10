// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EveAIHelpers.h"
#include "ModularGameplayActors/GSCModularAIController.h"
#include "EveAIController.generated.h"

UCLASS()
class PROJECTEVE_API AEveAIController : public AGSCModularAIController
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY()
	FTimerHandle CheckForgottenActors_TimerHandle;

	UPROPERTY()
	FTimerHandle SeekAttackTarget_TimerHandle;

	UPROPERTY()
	float SeekAfterSightLost_Timer;
	
public:
	// Sets default values for this actor's properties
	AEveAIController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="KeyName")
	FName AttackTargetKeyName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="KeyName")
	FName StatesKeyName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="KeyName")
	FName PointOfInterestKeyName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="KeyName")
	FName AttackRadiusKeyName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="KeyName")
	FName DefendRadiusKeyName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="KeyName")
	AActor* AttackTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="KeyName")
	TArray<AActor*> KnownSeenActors;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION()
	void CheckIfForgottenSeenActor();

	UFUNCTION()
	void SeekAttackTarget();

	UFUNCTION()
	bool CanSenseActor(AActor* Actor, EAISense AISense, FAIStimulus& Stimulus);

	UFUNCTION()
	void Handle_SensedSight(AActor* SensedActor);
	
	UFUNCTION()
	void Handle_LostSight(const AActor* Actor);

	UFUNCTION()
	void Handle_SensedHearing(const FVector& Location);

	UFUNCTION()
	void Handle_SensedDamage(AActor* SensedActor);

	UFUNCTION()
	void Handle_ForgotActor(AActor* ForgottenActor);
	

public:

	UFUNCTION()
	void Pawn_OnDeath();

	UFUNCTION()
	void AIPerception_OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UFUNCTION(BlueprintCallable)
	void SetStateAs_Passive();

	UFUNCTION(BlueprintCallable)
	void SetStateAs_Attacking(AActor* SensedActor, bool bUseLastKnownAttackTarget);
	
	UFUNCTION(BlueprintCallable)
	void SetStateAs_Seeking(const FVector& Location);

	UFUNCTION(BlueprintCallable)
	void SetStateAs_Investigating(const FVector& Location);

	UFUNCTION(BlueprintCallable)
	void SetStateAs_Frozen();

	UFUNCTION(BlueprintCallable)
	void SetStateAs_Dead();

	UFUNCTION(BlueprintCallable, Category="AI")
	EAIStates GetCurrentState();

	UFUNCTION(BlueprintCallable, Category="AI")
	AActor* GetAttackTarget() const
	{
		return AttackTarget;
	}
};

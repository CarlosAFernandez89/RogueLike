// Fill out your copyright notice in the Description page of Project Settings.


#include "EveAICharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BrainComponent.h"
#include "EveAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "CollisionHandler/CCCollisionHandlerComponent.h"
#include "Components/GSCCoreComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectEve/GameModes/EveArcadeGameMode.h"


// Sets default values
AEveAICharacter::AEveAICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GSCCoreComponent = CreateDefaultSubobject<UGSCCoreComponent>(TEXT("GSCCoreComponent"));
	GSCCoreComponent->OnDeath.AddDynamic(this, &AEveAICharacter::OnDeath);

	CCCollisionHandler = CreateDefaultSubobject<UCCCollisionHandlerComponent>(TEXT("CollisionHandler"));
	CCCollisionHandler->OnCollisionActivated.AddDynamic(this, &AEveAICharacter::CollisionHandler_OnCollisionActivated);
	CCCollisionHandler->OnHit.AddDynamic(this, &AEveAICharacter::CollisionHandler_OnHit);
}

void AEveAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEveAICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Get AIController Reference
	AIController = CastChecked<AEveAIController>(NewController);
	
}

void AEveAICharacter::CollisionHandler_OnCollisionActivated(ECCCollisionPart CollisionPart)
{
	if(HasAuthority())
	{
		switch (CollisionPart) {
		case ECCCollisionPart::NONE:
			break;
		case ECCCollisionPart::PrimaryItem:
			break;
		case ECCCollisionPart::SecondaryItem:
			break;
		case ECCCollisionPart::BothHandItems:
			break;
		case ECCCollisionPart::LeftArm:
			break;
		case ECCCollisionPart::RightArm:
			break;
		case ECCCollisionPart::LeftLeg:
			break;
		case ECCCollisionPart::RightLeg:
			break;
		case ECCCollisionPart::Custom1: CCCollisionHandler->UpdateCollidingComponent(GetMesh(), GetMesh()->GetAllSocketNames());
			break;
		case ECCCollisionPart::Custom2:
			break;
		case ECCCollisionPart::Custom3:
			break;
		default: ;
		}
	}
}

void AEveAICharacter::CollisionHandler_OnHit(const FHitResult& HitResult, UPrimitiveComponent* CollidingComponent)
{
	if(HasAuthority())
	{
		FGameplayEventData GameplayEventData;
		GameplayEventData.Instigator = this;
		GameplayEventData.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);
	
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			this,
			FGameplayTag::RequestGameplayTag("Event.Montage"),
			GameplayEventData);
	}
}

AAIPatrolRoute* AEveAICharacter::GetAIPatrolRoute_Implementation()
{
	return PatrolRoute;
}

float AEveAICharacter::SetAIMovementSpeed_Implementation(EAIMovementSpeed MovementSpeed)
{
	switch (MovementSpeed)
	{
	case Idle: GetCharacterMovement()->MaxWalkSpeed = 0.f; 
		return 0;
	case Patrol: GetCharacterMovement()->MaxWalkSpeed = 200.f; 
		return 200.f;
	case Jog: GetCharacterMovement()->MaxWalkSpeed = 350.f; 
		return 350.f;
	case Chase: GetCharacterMovement()->MaxWalkSpeed = 425.f; 
		return 425.f;
	default: ;
	}
	return 0;
}

void AEveAICharacter::GetAIIdealRange_Implementation(float& AttackRadius, float& DefendRadius)
{
	AttackRadius = 175.f;
	DefendRadius = 300.f;
}

float AEveAICharacter::GetAICurrentHealth_Implementation()
{
	return GSCCoreComponent->GetHealth();
}

float AEveAICharacter::GetAIMaxHealth_Implementation()
{
	return GSCCoreComponent->GetMaxHealth();
}

bool AEveAICharacter::GetAIIsDead_Implementation()
{
	return GSCCoreComponent->GetHealth() <= 0.f;
}

void AEveAICharacter::AIStoreAttackTokens_Implementation(AActor* AttackTarget, int32 Amount)
{
	if(const int32 Count = ReservedAttackTokens.FindOrAdd(AttackTarget))
	{
		ReservedAttackTokens.Add(AttackTarget, Count + Amount);
	}
}

void AEveAICharacter::AIReturnAllAttackTokens_Implementation()
{
	if (ReservedAttackTokens.Num() > 0)
	{
		for (const TPair<AActor*, int32>& TokenPair : ReservedAttackTokens)
		{
			//if (const IAIInteraction* InteractionInterface = Cast<IAIInteraction>(TokenPair.Key))
			//{
			//	InteractionInterface->Execute_ReturnAttackToken(TokenPair.Key, TokenPair.Value);
			//}
		}
        
		ReservedAttackTokens.Empty(); // Assuming we want to clear the map after returning tokens
	}
}

void AEveAICharacter::NavLink_JumpToDestination_Implementation(FVector Destination)
{
	const FVector RealDestination = FVector(Destination.X, Destination.Y, Destination.Z + 125.f);
	FVector LaunchVelocity;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		LaunchVelocity,
		GetActorLocation(),
		RealDestination);

	LaunchCharacter(LaunchVelocity,true, true);
}

void AEveAICharacter::DeathEvent_Implementation()
{
	if(AEveArcadeGameMode* ArcadeGameMode = Cast<AEveArcadeGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		ArcadeGameMode->OnMonsterKilled();
	}
}


void AEveAICharacter::OnDeath_Implementation()
{
	AIController->SetStateAs_Dead();
	AIController->ClearFocus(EAIFocusPriority::Gameplay);
	AIController->GetBrainComponent()->StopLogic(TEXT("Death"));
	Execute_AIReturnAllAttackTokens(this);
	
	FGameplayTagContainer GameplayTagContainer;
	GameplayTagContainer.AddTag(FGameplayTag::RequestGameplayTag("GameplayEvent.Death"));
	
	UGSCGameplayAbility* GameplayAbility;
	
	if(GSCCoreComponent->ActivateAbilityByTags(
		GameplayTagContainer,
		GameplayAbility,
		true))
	{
		DeathEvent();
	}
}


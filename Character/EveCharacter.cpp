// Fill out your copyright notice in the Description page of Project Settings.


#include "EveCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/GSCAbilityInputBindingComponent.h"
#include "Components/GSCCoreComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEve/Equipment/EveEquipmentManagerComponent.h"
#include "ProjectEve/Equipment/EveQuickBarComponent.h"
#include "ProjectEve/Interaction/IInteractableTarget.h"
#include "ProjectEve/Interaction/InteractionComponent.h"
#include "ProjectEve/Player/EvePlayerController.h"


void AEveCharacter::DistanceCheckForInteract(const AActor* TouchedActor)
{
	if (!TouchedActor) return;  // Safety check

	// Get the bounding sphere radius of the TouchedActor
	const float Radius = TouchedActor->GetSimpleCollisionRadius();

	// Calculate the actual distance from the edge of the TouchedActor

	// Check if the character is within the interaction distance from the edge of the TouchedActor
	if (const float DistanceToActor = FVector::Distance(TouchedActor->GetActorLocation(), this->GetActorLocation()) - Radius; DistanceToActor <= InteractionDistance)
	{
		GetWorld()->GetTimerManager().ClearTimer(DistanceForInteract_TimerHandle);

		// Make sure to retrieve the interaction component properly, ensuring it's not null
		if (UInteractionComponent* InteractionComponent = Cast<UInteractionComponent>(TouchedActor->GetComponentByClass(UInteractionComponent::StaticClass())))
		{
			StopMovementImmediately_Implementation();
			FRotator LookAtRotation = UKismetMathLibrary::MakeRotFromX(GetLookAtDirection_Implementation());
			LookAtRotation.Pitch = 0.f;
			SetActorRotation(LookAtRotation);
			IInteractableTarget::Execute_InteractWithActor(InteractionComponent, this);
		}
		else
		{
			// Optionally handle the case where no interaction component is found
			UE_LOG(LogTemp, Warning, TEXT("No InteractionComponent found on TouchedActor."));
		}
	}
}

// Sets default values
AEveCharacter::AEveCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the character if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->TargetArmLength = 1200.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	CameraBoom->bInheritYaw = false;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 10.f;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->FieldOfView = 75.f;

	AbilityInputBindingComponent = CreateDefaultSubobject<UGSCAbilityInputBindingComponent>(TEXT("AbilityInputBindingComponent"));

	GSCCoreComponent = CreateDefaultSubobject<UGSCCoreComponent>(TEXT("GSCCoreComponent"));

	EquipmentManagerComponent = CreateDefaultSubobject<UEveEquipmentManagerComponent>(TEXT("EquipmentManagerComponent"));
	
	CameraZoomFactor = 1050.f;
	
	InteractionDistance = 100.f;

}

void AEveCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StatTags);
}

FVector AEveCharacter::GetLookAtDirection_Implementation()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PC->GetPawn())
	{
		FHitResult HitResult;
		if (PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult) && HitResult.bBlockingHit)
		{
			const FVector MyLocation = GetActorLocation();
			// Compute the direction from the character to the hit location
			const FVector Direction = (HitResult.ImpactPoint - MyLocation).GetSafeNormal();
			return Direction;
		}
	}

	return FVector::ZeroVector;
}

void AEveCharacter::StopMovementImmediately_Implementation()
{
	if(UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->StopMovementImmediately();
	}
}

bool AEveCharacter::IsDead_Implementation()
{
	return GSCCoreComponent->GetHealth() <= 0.f;
}

void AEveCharacter::OnClickedActor_Implementation(AActor* TouchedActor, FKey PressedKey)
{
	if(FVector::Distance(TouchedActor->GetActorLocation(), this->GetActorLocation()) > InteractionDistance)
	{
		if(Controller)
		{
			const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Controller->GetWorld());
			if (!NavSys)
			{
				return;
			}

			// Assuming the actor has a Static Mesh or Skeletal Mesh component
			const UStaticMeshComponent* StaticMeshComponent = TouchedActor->GetComponentByClass<UStaticMeshComponent>();
			if (!StaticMeshComponent)
			{
				return; // Safe check if there's no primary component
			}

			const FBoxSphereBounds Bounds = StaticMeshComponent->Bounds;
			const float Radius = Bounds.SphereRadius;
			
			FNavLocation NavLocation;

			// Try to find a navigable location within the actor's bounding radius
			if (NavSys->GetRandomPointInNavigableRadius(TouchedActor->GetActorLocation(), Radius, NavLocation))
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, NavLocation.Location);
				
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUFunction(this, FName("DistanceCheckForInteract"),TouchedActor);
				GetWorld()->GetTimerManager().SetTimer(DistanceForInteract_TimerHandle, TimerDelegate, GetWorld()->GetDeltaSeconds(), true);
			}
		}

	}
	else
	{
		IInteractableTarget::Execute_InteractWithActor(TouchedActor->GetComponentByClass<UInteractionComponent>(), this);
	}
}

void AEveCharacter::ClearInteractionDelegate_Implementation()
{
	if(DistanceForInteract_TimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DistanceForInteract_TimerHandle);
	}
}

UEveQuickBarComponent* AEveCharacter::GetQuickBarComponent_Implementation()
{
	if(const AEvePlayerController* EvePC = Cast<AEvePlayerController>(GetLocalViewingPlayerController()))
	{
		return EvePC->QuickBarComponent;
	}
	return nullptr;
}

UEveInventoryManagerComponent* AEveCharacter::GetInventoryManagerComponent_Implementation()
{
	if(const AEvePlayerController* EvePC = Cast<AEvePlayerController>(GetLocalViewingPlayerController()))
	{
		return EvePC->InventoryManagerComponent;
	}
	return nullptr;
}

FEveEquipmentList AEveCharacter::GetEquipmentList_Implementation()
{
	if(EquipmentManagerComponent)
	{
		return EquipmentManagerComponent->GetEquipmentList();
	}
	
	return FEveEquipmentList();
}

void AEveCharacter::SetEquippedItem_Implementation(UEveEquipmentInstance* EquipmentInstance)
{
	CurrentEquipmentInstance = EquipmentInstance;
}

UEveEquipmentInstance* AEveCharacter::GetEquippedItem_Implementation()
{
	return CurrentEquipmentInstance;
}

void AEveCharacter::AddStatTagStack_Implementation(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void AEveCharacter::RemoveStatTagStack_Implementation(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 AEveCharacter::GetStatTagStackCount_Implementation(FGameplayTag Tag)
{
	return StatTags.GetStackCount(Tag);
}

bool AEveCharacter::HasStatTag_Implementation(FGameplayTag Tag)
{
	return StatTags.ContainsTag(Tag);
}

void AEveCharacter::OnNormalDamageDone_Implementation(const float DamageDone)
{
	FGameplayEventData EventData;
	EventData.EventMagnitude = DamageDone;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTag::RequestGameplayTag("GameplayEvent.Damage.Basic"), EventData);
}

void AEveCharacter::OnCriticalDamageDone_Implementation(const float DamageDone)
{
	FGameplayEventData EventData;
	EventData.EventMagnitude = DamageDone;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTag::RequestGameplayTag("GameplayEvent.Damage.Critical"), EventData);

}

void AEveCharacter::OnDamageTaken_Implementation(const float DamageTaken, const AActor* InstigatorActor)
{
	FGameplayEventData EventData;
	EventData.EventMagnitude = DamageTaken;
	EventData.Instigator = InstigatorActor;
	EventData.Target = this;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTag::RequestGameplayTag("GameplayEvent.Damage.Taken"), EventData);

}

AEvePlayerController* AEveCharacter::GetEvePlayerController_Implementation()
{
	if(AEvePlayerController* PlayerController = Cast<AEvePlayerController>(GetController()))
	{
		return PlayerController;
	}

	return nullptr;
}

void AEveCharacter::CameraZoom(const float ZoomFactor)
{
	if(CameraZoomTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(CameraZoomTimerHandle);
	}

	float MinValue;
	float MaxValue;
	CameraZoomCurve->GetTimeRange(MinValue, MaxValue);
	FTimerDelegate TimerDelegate;
	
	if(ZoomFactor > 0)
	{
		int32 TargetArmLength = UKismetMathLibrary::Clamp(CameraBoom->TargetArmLength - CameraZoomFactor,MinValue, MaxValue);
		FRotator Rotator = FRotator(CameraZoomCurve->GetFloatValue(TargetArmLength) + CameraBoomRotator_DefaultOffset.Pitch,
			CameraBoomRotator_DefaultOffset.Yaw,
			CameraBoomRotator_DefaultOffset.Roll);
		
		TimerDelegate.BindUFunction(this, FName("CameraZoomMovement"),
			TargetArmLength,
			Rotator);
	}
	else
	{
		int32 TargetArmLength = UKismetMathLibrary::Clamp(CameraBoom->TargetArmLength + CameraZoomFactor,MinValue, MaxValue);
		FRotator Rotator = FRotator(CameraZoomCurve->GetFloatValue(TargetArmLength) + CameraBoomRotator_DefaultOffset.Pitch,
			CameraBoomRotator_DefaultOffset.Yaw,
			CameraBoomRotator_DefaultOffset.Roll);
		
		TimerDelegate.BindUFunction(this, FName("CameraZoomMovement"),
			TargetArmLength,
			Rotator);
		
	}

	GetWorld()->GetTimerManager().SetTimer(CameraZoomTimerHandle, TimerDelegate, GetWorld()->GetDeltaSeconds(), true);

}

void AEveCharacter::CameraZoomMovement(const int32& TargetArmLength, const FRotator& Rotation)
{
	CameraBoom->TargetArmLength = FMath::FInterpTo(
		CameraBoom->TargetArmLength,
		TargetArmLength,
		GetWorld()->GetDeltaSeconds(),
		0.5f
		);
	
	CameraBoom->SetRelativeRotation(UKismetMathLibrary::RInterpTo(
		CameraBoom->GetRelativeRotation(),
		Rotation,
		GetWorld()->GetDeltaSeconds(),
		0.5
		));

	if(UKismetMathLibrary::NearlyEqual_FloatFloat(CameraBoom->TargetArmLength, TargetArmLength, 1.f)
		&& UKismetMathLibrary::EqualEqual_RotatorRotator(CameraBoom->GetRelativeRotation(), Rotation, 1.f))
	{
		GetWorld()->GetTimerManager().ClearTimer(CameraZoomTimerHandle);
	}
}

// Called when the game starts or when spawned
void AEveCharacter::BeginPlay()
{
	Super::BeginPlay();

	CameraBoomRotator_DefaultOffset = CameraBoom->GetComponentRotation();

}

// Called every frame
void AEveCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEveCharacter::NotifyRestarted()
{
	Super::NotifyRestarted();
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeManager.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectEve/GameModes/EveArcadeGameMode.h"
#include "ProjectEve/Interaction/InteractionComponent.h"


// Sets default values
AArcadeManager::AArcadeManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("InteractionMesh");
	RootComponent = InteractionMeshComponent;

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("InteractionComponent");
	InteractionComponent->OnInteractWithActor.AddDynamic(this, &AArcadeManager::OnInteract);

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);

	SplineRadius = 500.0f;
}

// Called when the game starts or when spawned
void AArcadeManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArcadeManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdateSplineRadius();
}

// Called every frame
void AArcadeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArcadeManager::OnInteract(AEveCharacter* InteractActor)
{
	check(InteractActor)

	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld()))
	{
		IArcadeGameModeHelpers::Execute_StartArcadeWave(GameMode);
	}
}

template <typename T>
void AArcadeManager::SpawnActorAsync(UWorld* World, TSubclassOf<T> ActorClass, const FTransform& SpawnTransform, TFunction<void(T*)> Callback)
{
	if (!World)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid World context!"));
		}
		return;
	}

	if (!*ActorClass)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid ActorClass!"));
		}
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Attempting to spawn actor of class: %s at location: %s"), *ActorClass->GetName(), *SpawnTransform.GetLocation().ToString()));
	}

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [World, ActorClass, SpawnTransform, Callback]()
	{
		// Actor spawning must happen in GameThread due to UE requirements.
		AsyncTask(ENamedThreads::GameThread, [World, ActorClass, SpawnTransform, Callback]()
		{
			if (T* SpawnedActor = World->SpawnActor<T>(ActorClass, SpawnTransform))
			{
				if (Callback)
				{
					Callback(SpawnedActor);
				}
			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to spawn actor in async task!"));
				}
			}
		});
	});
}


void AArcadeManager::UpdateSplineRadius()
{
	constexpr int32 NumPoints = 16;
	constexpr float AngleStep = 2 * PI / NumPoints;

	SplineComponent->ClearSplinePoints(false);

	for (int32 i = 0; i < NumPoints; ++i)
	{
		const float Angle = i * AngleStep;
		FVector PointLocation(SplineRadius * FMath::Cos(Angle), SplineRadius * FMath::Sin(Angle), 0);
		SplineComponent->AddSplinePoint(PointLocation, ESplineCoordinateSpace::Local, false);
	}

	SplineComponent->UpdateSpline();
}

void AArcadeManager::SpawnActorsForCurrentWave(const int32 CurrentWave, float& ProgressBarFillPerKill)
{
	//set visibility and collision to off while wave has started.
	InteractionMeshComponent->SetVisibility(false);
	InteractionMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (!WaveDataTable)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("WaveDataTable is not set"));
		}
		return;
	}

	static const FString ContextString(TEXT("Wave Data Context"));
	
	// Find the row for the current wave
	if (FWaveData* WaveData = WaveDataTable->FindRow<FWaveData>(*FString::FromInt(CurrentWave), ContextString, true))
	{
		for (int32 i = 0; i < WaveData->NumberOfActorsToSpawn; ++i)
		{

			ProgressBarFillPerKill = 1.f / WaveData->NumberOfActorsToSpawn;
			
			const float Distance = FMath::RandRange(0.0f, SplineComponent->GetSplineLength());
			FTransform SpawnTransform;
			FVector SpawnLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
			SpawnTransform.SetLocation(SpawnLocation);

			// Calculate rotation to face the ArcadeManager
			FVector Direction = GetActorLocation() - SpawnLocation;
			FRotator Rotation = Direction.Rotation();
			SpawnTransform.SetRotation(Rotation.Quaternion());

			const int32 RandomIndex = FMath::RandRange(0, WaveData->ActorsToSpawn.Num() - 1);
			TSubclassOf<AActor> ActorToSpawn = WaveData->ActorsToSpawn[RandomIndex];
			
			// Calculate the multiplier from the curve
			float MaxHealthMultiplier = MaxHealthScalingCurveFloat ? MaxHealthScalingCurveFloat->GetFloatValue(CurrentWave) : 1.0f;
			float DamageMultiplier = DamageScalingCurveFloat ? DamageScalingCurveFloat->GetFloatValue(CurrentWave) : 1.f;

			
			// Use the SpawnActorAsync function to spawn the specific actor type
			SpawnActorAsync<AActor>(GetWorld(), ActorToSpawn, SpawnTransform, [ActorToSpawn, this, MaxHealthMultiplier, DamageMultiplier](AActor* SpawnedActor)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Actor %s spawned successfully!"), *SpawnedActor->GetName()));

				// Apply the gameplay effect
				if (ScalingGameplayEffect && SpawnedActor->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass()))
				{
					IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(SpawnedActor);
					UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
						
					FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ScalingGameplayEffect, 1.0f, AbilitySystemComponent->MakeEffectContext());
					if (SpecHandle.IsValid())
					{
						FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
						Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("SetByCaller.MaxHealth")), MaxHealthMultiplier);
						Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Damage")), DamageMultiplier);

						AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
					}
				}
				else if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to spawn actor!"));
				}
			});
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Wave data for wave %d not found"), CurrentWave));
		}
	}
}

void AArcadeManager::PrepareForNextWave()
{
	InteractionMeshComponent->SetVisibility(true);
	InteractionMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
}


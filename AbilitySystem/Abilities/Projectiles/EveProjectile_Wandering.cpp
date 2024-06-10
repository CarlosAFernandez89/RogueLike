// Fill out your copyright notice in the Description page of Project Settings.


#include "EveProjectile_Wandering.h"

#include "Components/SphereComponent.h"
#include "Curves/CurveVector.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AEveProjectile_Wandering::AEveProjectile_Wandering()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	
	// Create and configure the sphere collider
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->InitSphereRadius(15.0f);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SphereCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	RootComponent = SphereCollider;
	
	// Create and configure the projectile movement component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = RootComponent;
	ProjectileMovementComponent->InitialSpeed = 1000.f;
	ProjectileMovementComponent->MaxSpeed = 1000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;

	// Initialize the time variable
	Time = 0.f;

	// Default interpolation speed
	InterpSpeed = 5.0f;

	Lifespan = 10.f;

	
}

// Called when the game starts or when spawned
void AEveProjectile_Wandering::BeginPlay()
{
	Super::BeginPlay();

	// Store the initial location
	InitialLocation = GetActorLocation();
	SetLifeSpan(Lifespan);
}

// Called every frame
void AEveProjectile_Wandering::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePosition(DeltaTime);
}

void AEveProjectile_Wandering::UpdatePosition_Implementation(float DeltaTime)
{
	if (PathCurve && !ProjectileMovementComponent->HomingTargetComponent.IsValid())
	{
		Time += DeltaTime;

		// Reset time if it exceeds the total movement time
		if (Time > TotalMovementTime)
		{
			Time = 0.0f;
		}

		FVector TargetLocation = PathCurve->GetVectorValue(Time);

		// Offset the target location by the initial location
		TargetLocation += InitialLocation;

		// Only move in the X, Y plane
		TargetLocation.Z = InitialLocation.Z;

		// Interpolate to the target location
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, InterpSpeed);

		SetActorLocation(NewLocation);
	}
}


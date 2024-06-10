// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPatrolRoute.h"

#include "Components/SplineComponent.h"


// Sets default values
AAIPatrolRoute::AAIPatrolRoute()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolRoute"));
	SplineComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAIPatrolRoute::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AAIPatrolRoute::GetSplinePointAsWorldPosition()
{
	return SplineComponent->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}

void AAIPatrolRoute::IncrementPatrolRoute()
{
	PatrolIndex += PatrolDirection;

	if(PatrolIndex == (SplineComponent->GetNumberOfSplinePoints() - 1))
	{
		PatrolDirection = -1;
	}
	else
	{
		if(PatrolIndex == 0)
		{
			PatrolDirection = 1;
		}
	}
}

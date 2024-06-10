// Fill out your copyright notice in the Description page of Project Settings.


#include "EveEquipmentInstance.h"

#include "EveEquipmentDefinition.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEve/Character/EveCharacterHelper.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#endif // UE_WITH_IRIS

class FLifetimeProperty;
class UClass;
class USceneComponent;

UEveEquipmentInstance::UEveEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UWorld* UEveEquipmentInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}


void UEveEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}


APawn* UEveEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UEveEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}
	return Result;
}

void UEveEquipmentInstance::SpawnEquipmentActors(const TArray<FEveEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = Char->GetMesh();
		}

		for (const FEveEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);
		}
	}
}

void UEveEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void UEveEquipmentInstance::OnEquipped()
{
	IEveCharacterHelper::Execute_SetEquippedItem(GetPawn(), this);
	K2_OnEquipped();
}

void UEveEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}

FVector UEveEquipmentInstance::GetMuzzleLocation()
{
	for (const auto SpawnedActor : SpawnedActors)
	{
		if(const USkeletalMeshComponent* SkeletalMeshComponent = SpawnedActor->GetComponentByClass<USkeletalMeshComponent>())
		{
			if(FVector SocketLocation = SkeletalMeshComponent->GetSocketLocation("Muzzle"); SocketLocation != FVector())
			{
				return SocketLocation;
			}
			
		}
	}
	return FVector();
}

void UEveEquipmentInstance::OnRep_Instigator()
{
}

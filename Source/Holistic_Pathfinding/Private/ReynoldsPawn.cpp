// Fill out your copyright notice in the Description page of Project Settings.

#include "ReynoldsPawn.h"
#include "Components/ActorComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "ReynoldsSteeringComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "ReynoldsSteeringComponent.h"
#include "Engine/CollisionProfile.h"
#include "ReynoldsSteeringBehavior.h"
#include "RestSteeringBehavior.h"
#include "SeekSteeringBehavior.h"
#include "FleeSteeringBehavior.h"
#include "ArriveSteeringBehavior.h"
#include "PursueSteeringBehavior.h"
#include "EvadeSteeringBehavior.h"
#include "WanderSteeringBehavior.h"
#include "InterposeSteeringBehavior.h"
#include "AvoidObstaclesSteeringBehavior.h"
#include "AvoidCollisionsSteeringBehavior.h"
#include "PathFollowSteeringBehavior.h"
#include "ContainmentSteeringBehavior.h"
#include "SeparationSteeringBehavior.h"
#include "AlignmentSteeringBehavior.h"
#include "CohesionSteeringBehavior.h"
#include "FlockingSteeringBehavior.h"

#include "Runtime/Engine/Classes/Engine/Engine.h"

FName AReynoldsPawn::FloatingPawnMovementComponentName(TEXT("MoveComp"));

// Sets default values
AReynoldsPawn::AReynoldsPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FloatingPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(AReynoldsPawn::FloatingPawnMovementComponentName);
}


// Called when the game starts or when spawned
void AReynoldsPawn::BeginPlay()
{
	Super::BeginPlay();
	if (AIControllersList.Num() && AIControllersList[0])
	{
		AIControllersList[0]->OnPossess(this);
	}
}


void AReynoldsPawn::OnConstruction(const FTransform& Transform)
{
	
}

#if WITH_EDITOR
void AReynoldsPawn::PostInitProperties()
{
	Super::PostInitProperties();
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;
	
}

void AReynoldsPawn::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;

	if ("AIControllerTypesList" == PropertyChangedEvent.GetPropertyName().ToString())
	{
		int32 index = PropertyChangedEvent.GetArrayIndex(PropertyChangedEvent.GetPropertyName().ToString());
		if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear)
		{
			for (auto& controller : AIControllersList)
			{
				if (controller)
				{
					controller->Destroy();
				}
			}
			AIControllersList.Empty();
		}
		else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove)
		{
			AIControllersList[index]->Destroy();
			AIControllersList.RemoveAt(index);
			
		}
		else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd)
		{
			AIControllersList.Add(nullptr);
		}
		else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate)
		{
			if (AIControllerTypesList[index])
			{
				//AIControllersList.Insert(NewObject<AReynoldsAIController>(this, AIControllerTypesList[index].Get()), index+1);
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.Instigator =  GetInstigator();
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnInfo.OverrideLevel = GetLevel();
				SpawnInfo.Owner = this;
				AIControllersList.Insert(GetWorld()->SpawnActor<AReynoldsAIController>(AIControllerTypesList[index].Get(), GetActorLocation(), GetActorRotation(), SpawnInfo), index + 1);
			}
			else
			{
				AIControllersList.Insert(nullptr, index);
			}
		}
		else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet)
		{
			if (AIControllerTypesList[index])
			{
				if (AIControllersList[index])
				{
					AIControllersList[index]->Destroy();
				}
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.Instigator =  GetInstigator();
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnInfo.OverrideLevel = GetLevel();
				SpawnInfo.Owner = this;
				AIControllersList[index] = GetWorld()->SpawnActor<AReynoldsAIController>(AIControllerTypesList[index].Get(), GetActorLocation(), GetActorRotation(), SpawnInfo);
			}
			else
			{
				if (AIControllersList[index])
				{
					AIControllersList[index]->Destroy();
				}
				AIControllersList[index] = nullptr;
			}
		}
		else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::Redirected)
		{
			UE_LOG(LogTemp, Warning, TEXT("Redirected"));
		}
		for (int i = 0; i < AIControllersList.Num(); i++)
		{
			if (AIControllersList[i])
			{
				AIControllersList[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				//FString PathName(FString("BoidAIControllers/") + GetName());
				//AIControllersList[i]->SetFolderPath(*PathName);
			}
		}
	}
}
# endif

// Called every frame
void AReynoldsPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AReynoldsPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
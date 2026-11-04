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
#include "GameFramework/Controller.h"

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

// "SeekTrainAIController_C" becomes "Seek Train".
static FString BehaviorDisplayName(const AController& Controller)
{
	FString Name = Controller.GetClass()->GetName();
	Name.RemoveFromEnd(TEXT("_C"));
	Name.RemoveFromEnd(TEXT("AIController"));
	return FName::NameToDisplayString(Name, false);
}

// Non-negative so it never collides with INDEX_NONE, which the engine treats as an unkeyed message.
static int32 BehaviorMessageKey(const FString& DisplayName)
{
	return static_cast<int32>(GetTypeHash(DisplayName) & 0x7FFFFFFFu);
}

// Boids on each behavior, by message key. Returns to empty when a Play session ends: APawn::EndPlay unpossesses
// every pawn, and that reaches NotifyControllerChanged below.
static TMap<int32, int32> BoidsPerBehavior;

void AReynoldsPawn::NotifyControllerChanged()
{
	// Read before Super: APawn::NotifyControllerChanged ends by setting PreviousController to the current controller.
	AController* const Previous = PreviousController;
	Super::NotifyControllerChanged();

	if (!GEngine)
	{
		return;
	}

	// A behavior's line stays while any boid runs it, whatever order the boids switch in.
	if (Previous)
	{
		const int32 Key = BehaviorMessageKey(BehaviorDisplayName(*Previous));
		int32& Count = BoidsPerBehavior.FindOrAdd(Key);
		if (--Count <= 0)
		{
			BoidsPerBehavior.Remove(Key);
			GEngine->RemoveOnScreenDebugMessage(static_cast<uint64>(Key));
		}
	}
	if (const AController* Current = GetController())
	{
		const FString DisplayName = BehaviorDisplayName(*Current);
		const int32 Key = BehaviorMessageKey(DisplayName);
		if (++BoidsPerBehavior.FindOrAdd(Key) == 1)
		{
			GEngine->AddOnScreenDebugMessage(Key, 1.0e9f, FColor::White, DisplayName);
		}
	}
}
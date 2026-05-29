// Fill out your copyright notice in the Description page of Project Settings.

#include "ReynoldsAIController.h"
#include "GameFramework/Pawn.h"
#include "ReynoldsPawn.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"

AReynoldsAIController::AReynoldsAIController()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SteeringBehaviors = { CreateDefaultSubobject<URestSteeringBehavior>("RestBehavior"), CreateDefaultSubobject<USeekSteeringBehavior>("SeekBehavior"), CreateDefaultSubobject<UFleeSteeringBehavior>("FleeBehavior"), CreateDefaultSubobject<UArriveSteeringBehavior>("ArriveBehavior"), CreateDefaultSubobject<UPursueSteeringBehavior>("PursueBehavior"), CreateDefaultSubobject<UEvadeSteeringBehavior>("EvadeBehavior"), CreateDefaultSubobject<UWanderSteeringBehavior>("WanderBehavior"), CreateDefaultSubobject<UInterposeSteeringBehavior>("InterposeBehavior"), CreateDefaultSubobject<UAvoidObstaclesSteeringBehavior>("AvoidObstaclesBehavior"), CreateDefaultSubobject<UAvoidCollisionsSteeringBehavior>("AvoidCollisionsBehavior"), CreateDefaultSubobject<UPathFollowSteeringBehavior>("PathFollowBehavior"), CreateDefaultSubobject<UContainmentSteeringBehavior>("ContainmentBehavior"), CreateDefaultSubobject<USeparationSteeringBehavior>("SeparationBehavior"), CreateDefaultSubobject<UAlignmentSteeringBehavior>("AlignmentBehavior"), CreateDefaultSubobject<UCohesionSteeringBehavior>("CohesionBehavior"), CreateDefaultSubobject<UFlockingSteeringBehavior>("FlockingBehavior") };
	for (auto& behavior : SteeringBehaviors)
	{
		behavior->OwningController = this;
	}
}



void AReynoldsAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AReynoldsPawn* ReynoldsPawn = dynamic_cast<AReynoldsPawn*>(InPawn);

	if (GetPawn() && ReynoldsPawn == GetPawn())
	{
		for (auto& behavior : SteeringBehaviors)
		{
			behavior->OwningPawn = InPawn;
			behavior->Initialize();
		}
		if (bOverrideMovementComponentSettings)
		{
			UFloatingPawnMovement* FloatingPawnMovement = dynamic_cast<UFloatingPawnMovement*>(GetPawn()->GetMovementComponent());
			if (FloatingPawnMovement)
			{
				FloatingPawnMovement->MaxSpeed = MaxSpeed;
				FloatingPawnMovement->Acceleration = Acceleration;
				FloatingPawnMovement->Deceleration = Decceleration;
				FloatingPawnMovement->TurningBoost = TurningBoost;
			}
		}
	}
}

void AReynoldsAIController::OnUnPossess()
{
	Super::OnUnPossess();
	if (BrainComponent)
	{
		BrainComponent->Cleanup();
	}
	for (auto& behavior : SteeringBehaviors)
	{
		if (behavior) //If a controller is destroied OnUnPossess is called and on pressing compile in a blueprint the controller can be destroied without having valid steering behaviors.
		{
			behavior->OwningPawn = nullptr;
		}
	}
}

URestSteeringBehavior* AReynoldsAIController::GetRestBehavior()
{
	check(dynamic_cast<URestSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Rest)]));
	return dynamic_cast<URestSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Rest)]);
}

USeekSteeringBehavior* AReynoldsAIController::GetSeekBehavior()
{
	check(dynamic_cast<USeekSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Seek)]));
	return dynamic_cast<USeekSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Seek)]);
}

UFleeSteeringBehavior* AReynoldsAIController::GetFleeBehavior()
{
	check(dynamic_cast<UFleeSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Flee)]));
	return dynamic_cast<UFleeSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Flee)]);
}

UArriveSteeringBehavior* AReynoldsAIController::GetArriveBehavior()
{
	check(dynamic_cast<UArriveSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Arrive)]));
	return dynamic_cast<UArriveSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Arrive)]);
}

UPursueSteeringBehavior* AReynoldsAIController::GetPursueBehavior()
{
	check(dynamic_cast<UPursueSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Pursue)]));
	return dynamic_cast<UPursueSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Pursue)]);
}

UEvadeSteeringBehavior* AReynoldsAIController::GetEvadeBehavior()
{
	check(dynamic_cast<UEvadeSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Evade)]));
	return dynamic_cast<UEvadeSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Evade)]);
}

UWanderSteeringBehavior* AReynoldsAIController::GetWanderBehavior()
{
	check(dynamic_cast<UWanderSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Wander)]));
	return dynamic_cast<UWanderSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Wander)]);
}

UInterposeSteeringBehavior* AReynoldsAIController::GetInterposeBehavior()
{
	check(dynamic_cast<UInterposeSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Interpose)]));
	return dynamic_cast<UInterposeSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Interpose)]);
}

UAvoidObstaclesSteeringBehavior* AReynoldsAIController::GetAvoidObstaclesBehavior()
{
	check(dynamic_cast<UAvoidObstaclesSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::AvoidObstacles)]));
	return dynamic_cast<UAvoidObstaclesSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::AvoidObstacles)]);
}

UAvoidCollisionsSteeringBehavior* AReynoldsAIController::GetAvoidCollisionsBehavior()
{
	check(dynamic_cast<UAvoidCollisionsSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::AvoidCollisions)]));
	return dynamic_cast<UAvoidCollisionsSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::AvoidCollisions)]);
}

UPathFollowSteeringBehavior* AReynoldsAIController::GetPathFollowBehavior()
{
	check(dynamic_cast<UPathFollowSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::PathFollow)]));
	return dynamic_cast<UPathFollowSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::PathFollow)]);
}

UContainmentSteeringBehavior* AReynoldsAIController::GetContainmentBehavior()
{
	check(dynamic_cast<UContainmentSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Containment)]));
	return dynamic_cast<UContainmentSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Containment)]);
}

USeparationSteeringBehavior* AReynoldsAIController::GetSeparationBehavior()
{
	check(dynamic_cast<USeparationSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Separation)]));
	return dynamic_cast<USeparationSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Separation)]);
}

UAlignmentSteeringBehavior* AReynoldsAIController::GetAlignmentBehavior()
{
	check(dynamic_cast<UAlignmentSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Alignment)]));
	return dynamic_cast<UAlignmentSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Alignment)]);
}

UCohesionSteeringBehavior* AReynoldsAIController::GetCohesionBehavior()
{
	check(dynamic_cast<UCohesionSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Cohesion)]));
	return dynamic_cast<UCohesionSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Cohesion)]);
}

UFlockingSteeringBehavior* AReynoldsAIController::GetFlockingBehavior()
{
	check(dynamic_cast<UFlockingSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Flocking)]));
	return dynamic_cast<UFlockingSteeringBehavior*>(SteeringBehaviors[static_cast<int>(ESteeringBehavior::Flocking)]);
}

void AReynoldsAIController::AddBehaviorToCurrentSteeringBehavior(UReynoldsSteeringBehavior* newBehavior)
{
	CurrentSteeringBehavior.AddUnique(newBehavior);
}

void AReynoldsAIController::ClearCurrentSteeringBehaviors()
{
	CurrentSteeringBehavior.Reset();
}

//#pragma optimize("", on)
FVector AReynoldsAIController::CalculateCurrentSteeringVector()
{
	if (GetPawn())
	{

		CurrentSteeringVector = FVector(0, 0, 0);
		for (auto& behavior : CurrentSteeringBehavior)
		{
			FVector CurrentVector;
			if (behavior->CalculateSteeringVector(CurrentVector))
			{
				CurrentSteeringVector += CurrentVector * behavior->Weight;
			}
		}
		CurrentSteeringVector = CurrentSteeringVector.GetClampedToMaxSize(GetPawn()->GetMovementComponent()->GetMaxSpeed());
	}
	return CurrentSteeringVector;
}

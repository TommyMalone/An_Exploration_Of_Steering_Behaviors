// Fill out your copyright notice in the Description page of Project Settings.

#include "PathFollowSteeringBehavior.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"

bool UPathFollowSteeringBehavior::CalculateSteeringVector(FVector & SteeringVector)
{

	if (Path)
	{
		return FollowPath(SteeringVector, Path, PathRadius, LookAheadDistance);
	}
	else if (PathActor)
	{
		USplineComponent* SpecifiedSpline = dynamic_cast<USplineComponent*>(PathActor->GetDefaultSubobjectByName(SplineComponentName));
		if (SpecifiedSpline)
		{
			Path = SpecifiedSpline;
		}
		else
		{
			Path = PathActor->FindComponentByClass<USplineComponent>();
		}
	}
	return false;
}

bool UPathFollowSteeringBehavior::IsOnPath()
{
	bool isOnPath = false;
	if (OwningPawn && Path)
	{
		FVector PredictedLocation = OwningPawn->GetActorLocation() + OwningPawn->GetMovementComponent()->Velocity.GetSafeNormal()*LookAheadDistance;
		FVector ClosestPointOnPathToPredicted = Path->FindLocationClosestToWorldLocation(PredictedLocation, ESplineCoordinateSpace::World);
		if (((PredictedLocation - ClosestPointOnPathToPredicted).Size() < PathRadius))
		{
			isOnPath = true;
		}
	}
	return isOnPath;
}

void UPathFollowSteeringBehavior::Initialize()
{
	if (PathActor)
	{
		USplineComponent* SpecifiedSpline = dynamic_cast<USplineComponent*>(PathActor->GetDefaultSubobjectByName(SplineComponentName));
		if (SpecifiedSpline)
		{
			Path = SpecifiedSpline;
		}
		else
		{
			Path = PathActor->FindComponentByClass<USplineComponent>();
		}
	}
	else
	{
		Path = nullptr;
	}
}

void UPathFollowSteeringBehavior::PostLoad()
{
	Super::PostLoad();

	if (PathActor)
	{
		USplineComponent* SpecifiedSpline = dynamic_cast<USplineComponent*>(PathActor->GetDefaultSubobjectByName(SplineComponentName));
		if (SpecifiedSpline)
		{
			Path = SpecifiedSpline;
		}
		else
		{
			Path = PathActor->FindComponentByClass<USplineComponent>();
		}
	}
	else
	{
		Path = nullptr;
	}
}

void UPathFollowSteeringBehavior::PostInitProperties()
{
	Super::PostInitProperties();

	if (PathActor)
	{
		USplineComponent* SpecifiedSpline = dynamic_cast<USplineComponent*>(PathActor->GetDefaultSubobjectByName(SplineComponentName));
		if (SpecifiedSpline)
		{
			Path = SpecifiedSpline;
		}
		else
		{
			Path = PathActor->FindComponentByClass<USplineComponent>();
		}
	}
	else
	{
		Path = nullptr;
	}
}

void UPathFollowSteeringBehavior::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PathActor)
	{
		USplineComponent* SpecifiedSpline = dynamic_cast<USplineComponent*>(PathActor->GetDefaultSubobjectByName(SplineComponentName));
		if (SpecifiedSpline)
		{
			Path = SpecifiedSpline;
		}
		else
		{
			Path = PathActor->FindComponentByClass<USplineComponent>();
		}
	}
	else
	{
		Path = nullptr;
	}
}

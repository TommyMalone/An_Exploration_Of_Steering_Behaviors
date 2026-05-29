// Fill out your copyright notice in the Description page of Project Settings.

#include "FleeSteeringBehavior.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Pawn.h"

bool UFleeSteeringBehavior::CalculateSteeringVector(FVector & SteeringVector)
{
	if (FleeTarget)
	{
		return Flee(SteeringVector, FleeTarget->GetActorLocation());
	}
	return Flee(SteeringVector, FleeDestination);
}



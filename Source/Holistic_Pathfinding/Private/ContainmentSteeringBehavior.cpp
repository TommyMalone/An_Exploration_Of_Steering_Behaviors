// Fill out your copyright notice in the Description page of Project Settings.

#include "ContainmentSteeringBehavior.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Pawn.h"

bool UContainmentSteeringBehavior::CalculateSteeringVector(FVector & SteeringVector)
{
	return ContainInActor(SteeringVector, ActorContainer);
}

bool UContainmentSteeringBehavior::IsContained()
{
	bool IsContained = false;
	if (OwningPawn && ActorContainer && ActorContainer->IsOverlappingActor(OwningPawn))
	{
		IsContained = true;
	}
	return IsContained;
}

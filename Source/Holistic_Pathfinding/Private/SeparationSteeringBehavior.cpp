// Fill out your copyright notice in the Description page of Project Settings.

#include "SeparationSteeringBehavior.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Pawn.h"

bool USeparationSteeringBehavior::CalculateSteeringVector(FVector & SteeringVector)
{
	return Separation(SteeringVector, OwningPawn, VisionDistance, VisionAngle, ObjectTypes, ActorClassFilter, ActorsToIgnore);
}



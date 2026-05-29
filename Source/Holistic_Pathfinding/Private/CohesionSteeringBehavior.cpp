// Fill out your copyright notice in the Description page of Project Settings.

#include "CohesionSteeringBehavior.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Pawn.h"

bool UCohesionSteeringBehavior::CalculateSteeringVector(FVector & SteeringVector)
{
	return Cohesion(SteeringVector, this, VisionDistance, VisionAngle, ObjectTypes, ActorClassFilter, ActorsToIgnore);
}



// Fill out your copyright notice in the Description page of Project Settings.

#include "FlockingSteeringBehavior.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Pawn.h"

bool UFlockingSteeringBehavior::CalculateSteeringVector(FVector & SteeringVector)
{
	return Flocking(SteeringVector, this, AlignmentWeight, AlignmentVisionDistance, AlignmentVisionAngle, CohesionWeight, CohesionVisionDistance, CohesionVisionAngle, SeparationWeight, SeparationVisionDistance, SeparationVisionAngle,ObjectTypes, ActorClassFilter, ActorsToIgnore);
}



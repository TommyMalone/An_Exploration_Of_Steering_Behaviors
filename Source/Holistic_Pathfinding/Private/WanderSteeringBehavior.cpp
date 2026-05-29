// Fill out your copyright notice in the Description page of Project Settings.

#include "WanderSteeringBehavior.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Pawn.h"

bool UWanderSteeringBehavior::CalculateSteeringVector(FVector & SteeringVector)
{
	return Wander(SteeringVector, SphereDistance, SphereRadius);
}



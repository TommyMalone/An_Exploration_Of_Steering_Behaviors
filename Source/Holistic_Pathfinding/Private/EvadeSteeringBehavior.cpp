// Fill out your copyright notice in the Description page of Project Settings.

#include "EvadeSteeringBehavior.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Pawn.h"

bool UEvadeSteeringBehavior::CalculateSteeringVector(FVector & SteeringVector)
{
	return Evade(SteeringVector, Pursuer);
}



// Fill out your copyright notice in the Description page of Project Settings.

#include "PursueSteeringBehavior.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Pawn.h"

bool UPursueSteeringBehavior::CalculateSteeringVector(FVector & SteeringVector)
{
	if (Evader)
	{
		return Pursue(SteeringVector, Evader, Offset);
	}
	return false;
}

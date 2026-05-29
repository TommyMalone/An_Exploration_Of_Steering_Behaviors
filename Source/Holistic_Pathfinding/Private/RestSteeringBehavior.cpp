// Fill out your copyright notice in the Description page of Project Settings.

#include "RestSteeringBehavior.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Pawn.h"


bool URestSteeringBehavior::CalculateSteeringVector(FVector & SteeringVector)
{
	SteeringVector = FVector(0, 0, 0);
	return true;
}



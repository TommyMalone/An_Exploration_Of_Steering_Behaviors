// Fill out your copyright notice in the Description page of Project Settings.

#include "SeekSteeringBehavior.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Pawn.h"


bool USeekSteeringBehavior::CalculateSteeringVector(FVector & SteeringVector)
{
	if (SeekTarget)
	{
		return Seek(SteeringVector, SeekTarget->GetActorLocation());
	}
	return Seek(SteeringVector, Destination);
}
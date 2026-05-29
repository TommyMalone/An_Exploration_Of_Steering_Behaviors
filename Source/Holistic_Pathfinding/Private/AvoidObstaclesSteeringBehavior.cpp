// Fill out your copyright notice in the Description page of Project Settings.

#include "AvoidObstaclesSteeringBehavior.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Pawn.h"

bool UAvoidObstaclesSteeringBehavior::CalculateSteeringVector(FVector & SteeringVector)
{
	check(TraceType != ETraceType::ENDENUM);
	if (TraceType == ETraceType::Object)
	{
		return CylindricalObstacleAvoidanceForObjects(SteeringVector, OwningPawn, LookAheadDistance, Radius, ObjectTypes, bTraceComplex, ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor);
	}
	else
	{
		return CylindricalObstacleAvoidanceByChannel(SteeringVector, OwningPawn, LookAheadDistance, Radius, ChannelTypes, bTraceComplex, ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor);
	}
}



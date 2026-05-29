// Fill out your copyright notice in the Description page of Project Settings.

#include "ArriveSteeringBehavior.h"

bool UArriveSteeringBehavior::CalculateSteeringVector(FVector & SteeringVector)
{
	if (ArriveTarget)
	{
		return Arrive(SteeringVector, ArriveTarget->GetActorLocation(), bIsWithinArrivalRadius, ArrivalRadius);
	}
	return Arrive(SteeringVector, Destination, bIsWithinArrivalRadius, ArrivalRadius);
}

bool UArriveSteeringBehavior::IsWithinArrivalRadius()
{
	float Distance = FVector::Dist(Destination, OwningPawn->GetActorLocation());

	if (Distance < ArrivalRadius)
	{
		bIsWithinArrivalRadius = true;
	}
	else
	{
		bIsWithinArrivalRadius = false;
	}
	return bIsWithinArrivalRadius;
}

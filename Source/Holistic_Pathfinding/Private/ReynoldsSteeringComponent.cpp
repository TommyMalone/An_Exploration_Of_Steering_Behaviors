// Fill out your copyright notice in the Description page of Project Settings.

#include "ReynoldsSteeringComponent.h"
#include "Runtime/Engine/Public/WorldCollision.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"

//#pragma optimize("", off)
//#pragma optimize("", on)

// Sets default values for this component's properties
UReynoldsSteeringComponent::UReynoldsSteeringComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	//MovementComponent = GetOwner()->FindComponentByClass<UMovementComponent>();
	
}


// Called when the game starts
void UReynoldsSteeringComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

UFloatingPawnMovement const * UReynoldsSteeringComponent::GetMovementComponent() const
{
	return MovementComponent;
}

void UReynoldsSteeringComponent::SetMovementComponent(UFloatingPawnMovement * Component)
{
	MovementComponent = Component;
}

// Called every frame
void UReynoldsSteeringComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector UReynoldsSteeringComponent::GetVelocity()
{
	return MovementComponent ? MovementComponent->Velocity : FVector(0, 0, 0);
}

float UReynoldsSteeringComponent::GetMaxSpeed()
{
	return MovementComponent ? MovementComponent->MaxSpeed : 0;
}

float UReynoldsSteeringComponent::GetAcceleration()
{
	return MovementComponent ? MovementComponent->Acceleration : 0;
}

float UReynoldsSteeringComponent::GetDeceleration()
{
	return MovementComponent ? MovementComponent->Deceleration : 0;
}

float UReynoldsSteeringComponent::GetTurnBoost()
{
	return MovementComponent ? MovementComponent->TurningBoost : 0;
}

FVector UReynoldsSteeringComponent::ClampSteeringToMaxSpeed(FVector const & SteeringVector)
{
	return SteeringVector.GetClampedToMaxSize(GetMaxSpeed());
}

FVector UReynoldsSteeringComponent::ScaleSteeringToInputVector(FVector const & SteeringVector)
{
	return SteeringVector.GetClampedToMaxSize(GetMaxSpeed()) / GetMaxSpeed();
}

bool UReynoldsSteeringComponent::Seek(FVector & SteeringVector, FVector const & Destination)
{
	SteeringVector = (Destination - GetOwner()->GetActorLocation()).GetSafeNormal() * GetMaxSpeed() - GetVelocity();
	return true;
}

bool UReynoldsSteeringComponent::Flee(FVector & SteeringVector, FVector const & FleeDestination)
{
	SteeringVector = (GetOwner()->GetActorLocation() - FleeDestination).GetSafeNormal() * GetMaxSpeed() - GetVelocity();
	return true;
}

//TODO Add curve as optional parameter
bool UReynoldsSteeringComponent::Arrive(FVector & SteeringVector, FVector const & Destination, bool & IsWithinArrivalRadius, float ArrivalRadius)
{
	bool RecalculatedSteering = Seek(SteeringVector, Destination);

	float Distance = FVector::Dist(Destination, GetOwner()->GetActorLocation());

	if (ArrivalRadius < 0)
	{
		ArrivalRadius = GetMaxSpeed();
	}

	if (Distance <= ArrivalRadius)
	{
		SteeringVector = SteeringVector * (Distance / ArrivalRadius);
		IsWithinArrivalRadius = true;
	}

	return RecalculatedSteering;
}

#pragma optimize("", off)
bool UReynoldsSteeringComponent::Pursue(FVector & SteeringVector, AActor const * Evader, float Offset)
{
	bool RecalculatedSteering = false;
	if (Evader && IsValid(Evader) && Evader->GetRootComponent())
	{
		if (Offset < 0)
		{
			Offset = GetMaxSpeed();
		}
		FVector MyLocation = GetOwner()->GetActorLocation();
		FVector TargetLocation = Evader->GetActorLocation();
		float PredictionInterval = FVector::Dist(TargetLocation, MyLocation) / GetMaxSpeed();
		FVector PredictedLocation = TargetLocation + Evader->GetVelocity() * PredictionInterval;

		FVector OffsetTargetLocation = TargetLocation + (TargetLocation - MyLocation).GetSafeNormal() * Offset;
		FVector OffsetPredictedLocation = PredictedLocation + (TargetLocation - PredictedLocation).GetSafeNormal() * Offset;

		if ((PredictedLocation - MyLocation).Size() < (TargetLocation - MyLocation).Size())
		{
			RecalculatedSteering = Seek(SteeringVector, OffsetPredictedLocation);
		}
		else
		{
			RecalculatedSteering = Seek(SteeringVector, OffsetTargetLocation);
		}
	}
	return RecalculatedSteering;
}
#pragma optimize("", on)

bool UReynoldsSteeringComponent::Evade(FVector & SteeringVector, AActor const * Pursuer)
{
	bool RecalculatedSteering = false;
	if (Pursuer && IsValid(Pursuer) && Pursuer->GetRootComponent())
	{
		FVector MyLocation = GetOwner()->GetActorLocation();
		FVector TargetLocation = Pursuer->GetActorLocation();
		float PredictionInterval = FVector::Dist(TargetLocation, MyLocation) / GetMaxSpeed();
		FVector PredictedLocation = TargetLocation + Pursuer->GetVelocity() * PredictionInterval;

		//This will prevent the evade from evading into the pursuer, but it also lead to flee behavior (rather than evade behavior) far too often.
		/*if ((PredictedLocation - MyLocation).Size() > (TargetLocation - MyLocation).Size())
		{
			FleeDestination = PredictedLocation;
		}
		else
		{
			FleeDestination = TargetLocation;
		}
		*/
		RecalculatedSteering = Flee(SteeringVector, PredictedLocation);
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringComponent::Wander(FVector & SteeringVector, float SphereDistance, float SphereRadius)
{
	bool RecalculatedSteering = false;
	if (SphereRadius < 0)
	{
		SphereRadius = GetMaxSpeed();
	}
	FVector SphereCenterLocation = GetVelocity().GetSafeNormal()*SphereDistance;
	FVector WanderDestination = GetOwner()->GetActorLocation() + SphereCenterLocation + FMath::VRand() * SphereRadius;

	RecalculatedSteering = Seek(SteeringVector, WanderDestination);
	return RecalculatedSteering;
}

bool UReynoldsSteeringComponent::Interpose(FVector & SteeringVector, TArray<AActor *> ActorArray, TArray<float> ActorWeightsArray)
{
	bool RecalculatedSteering = false;
	if (ActorWeightsArray.Num() >= 2 && ActorWeightsArray.Num() <= ActorArray.Num())
	{
		float SumOfWeights = 0;
		FVector MyLocation = GetOwner()->GetActorLocation();

		for (int32 i = 0; i < ActorWeightsArray.Num(); i++)
		{
			AActor const * currentActor = ActorArray[i];
			if (currentActor && IsValid(currentActor) && currentActor->GetRootComponent())
			{
				SumOfWeights += ActorWeightsArray[i];
			}
		}

		FVector AverageWeightedActorLocation(0, 0, 0);

		for (int32 i = 0; i < ActorWeightsArray.Num(); i++)
		{
			AActor * const currentActor = ActorArray[i];
			if (currentActor && IsValid(currentActor) && currentActor->GetRootComponent())
			{
				AverageWeightedActorLocation += currentActor->GetActorLocation() * ActorWeightsArray[i] / SumOfWeights;
			}
		}

		float PredictionInterval = FVector::Dist(AverageWeightedActorLocation, MyLocation) / GetMaxSpeed();

		FVector AverageWeightedPredictedActorLocation(0, 0, 0);

		for (int32 i = 0; i < ActorArray.Num(); i++)
		{
			AActor const * currentActor = ActorArray[i];
			if (currentActor && IsValid(currentActor) && currentActor->GetRootComponent())
			{
				AverageWeightedPredictedActorLocation += (currentActor->GetActorLocation() + currentActor->GetVelocity() * PredictionInterval) * ActorWeightsArray[i] / SumOfWeights;
			}
		}

		if (SumOfWeights > 0)
		{
			RecalculatedSteering = Seek(SteeringVector, AverageWeightedPredictedActorLocation);
		}
	}
	else if (ActorArray.Num() >= 2)
	{
		FVector AveragePredictedLocation(0, 0, 0);
		FVector AverageActorLocation = UGameplayStatics::GetActorArrayAverageLocation(ActorArray);
		FVector MyLocation = GetOwner()->GetActorLocation();
		float PredictionInterval = FVector::Dist(AverageActorLocation, MyLocation) / GetMaxSpeed();

		int32 NumActors = 0;

		for (int32 i = 0; i < ActorArray.Num(); i++)
		{
			AActor * currentActor = ActorArray[i];
			if (currentActor && IsValid(currentActor) && currentActor->GetRootComponent())
			{
				AveragePredictedLocation += currentActor->GetActorLocation() + currentActor->GetVelocity() * PredictionInterval;
				NumActors++;
			}
		}

		if (NumActors > 0)
		{
			AveragePredictedLocation /=  NumActors;
			RecalculatedSteering = Seek(SteeringVector, AveragePredictedLocation);
		}
	}
	return RecalculatedSteering;
}



bool UReynoldsSteeringComponent::CylindricalObstacleAvoidanceByChannel(FVector & SteeringVector, UObject * WorldContextObject, float LookAheadDistance, float Radius, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult & OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	bool RecalculatedSteering = false;
	if (GetVelocity().Size() > 0)
	{
		FVector Start = GetOwner()->GetActorLocation() + GetVelocity().GetSafeNormal()*(Radius);
		FVector End = Start + (Start - GetOwner()->GetActorLocation()).GetSafeNormal()* (LookAheadDistance - 2 * Radius);
		if (UKismetSystemLibrary::SphereTraceSingle(WorldContextObject, Start, End, Radius, TraceChannel, bTraceComplex, ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime))
		{
			SteeringVector = OutHit.ImpactNormal*GetMaxSpeed()*OutHit.Distance/LookAheadDistance;
			RecalculatedSteering = true;
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringComponent::CylindricalObstacleAvoidanceForObjects(FVector & SteeringVector, UObject * WorldContextObject, float LookAheadDistance, float Radius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult & OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	bool RecalculatedSteering = false;
	if (GetVelocity().Size() > 0)
	{
		FVector Start = GetOwner()->GetActorLocation() + GetVelocity().GetSafeNormal()*(Radius);
		FVector End = Start + ((Start - GetOwner()->GetActorLocation()).GetSafeNormal())*(LookAheadDistance - Radius);
		if (UKismetSystemLibrary::SphereTraceSingleForObjects(WorldContextObject, Start, End, Radius, ObjectTypes, bTraceComplex, ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime))
		{
			SteeringVector = OutHit.Normal*GetMaxSpeed();
			RecalculatedSteering = true;
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringComponent::UnalignedCollisionAvoidance(FVector & SteeringVector, float AvoidanceDistance, TArray<AActor*> ActorArray)
{
	bool RecalculatedSteering = false;
	for (auto & Actor : ActorArray)
	{
		FVector DifferenceInPositions = GetOwner()->GetActorLocation() - Actor->GetActorLocation();
		FVector DifferenceInVelocity = ((GetVelocity() - Actor->GetVelocity()));
		if (DifferenceInVelocity.Size())
		{
			float TimeOfClosestApproach = FVector::DotProduct(-DifferenceInPositions, DifferenceInVelocity) / DifferenceInVelocity.SizeSquared();
			if (TimeOfClosestApproach > 0)
			{
				FVector ThisPositionAtClosestApproach = GetOwner()->GetActorLocation() + GetVelocity()*TimeOfClosestApproach;
				FVector OtherPositionAtClosestApproach = Actor->GetActorLocation() + Actor->GetVelocity()*TimeOfClosestApproach;
				if ((OtherPositionAtClosestApproach - ThisPositionAtClosestApproach).Size() < AvoidanceDistance)
				{
					SteeringVector = (ThisPositionAtClosestApproach - OtherPositionAtClosestApproach).GetSafeNormal()*GetMaxSpeed();
					RecalculatedSteering = true;
				}
			}
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringComponent::FollowPath(FVector & SteeringVector, USplineComponent const * Path, float PathRadius, float LookAheadDistance)
{
	bool RecalculatedSteering = false;
	FVector PredictedLocation = GetOwner()->GetActorLocation() + GetVelocity().GetSafeNormal()*LookAheadDistance;
	FVector ClosestPointOnPathToPredicted = Path->FindLocationClosestToWorldLocation(PredictedLocation, ESplineCoordinateSpace::World);
	if ((PredictedLocation - ClosestPointOnPathToPredicted).Size() > PathRadius)
	{
		FVector DestinationOnPath = ClosestPointOnPathToPredicted + Path->FindDirectionClosestToWorldLocation(PredictedLocation, ESplineCoordinateSpace::World) * LookAheadDistance;
		RecalculatedSteering = Seek(SteeringVector, DestinationOnPath);
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringComponent::FollowWall(FVector & SteeringVector)
{
	return false;
}

bool UReynoldsSteeringComponent::ContainInActor(FVector & SteeringVector, AActor * Container)
{
	bool RecalculatedSteering = false;
	if (Container && !Container->IsOverlappingActor(GetOwner()))
	{
		float ShortestDistance = TNumericLimits<float>::Max();
		float currentDistance = 0;
		FVector DestinationCandidate(0, 0, 0);
		TArray<UPrimitiveComponent*> Primitives;
		Container->GetComponents<UPrimitiveComponent>(Primitives);
		if (Primitives.Num())
		{
			for (auto & Primitive : Primitives)
			{
				currentDistance = Primitive->GetClosestPointOnCollision(GetOwner()->GetActorLocation(), DestinationCandidate);
				if (currentDistance < ShortestDistance)
				{
					ShortestDistance = currentDistance;
				}
			}
			RecalculatedSteering = Seek(SteeringVector, DestinationCandidate);
		}

	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringComponent::ContainInPrimitive(FVector & SteeringVector, UPrimitiveComponent * Container)
{
	bool RecalculatedSteering = false;
	if (!Container->IsOverlappingActor(GetOwner()))
	{
		float ShortestDistance = TNumericLimits<float>::Max();
		FVector ClosestPointOnCollision;
		float currentDistance = Container->GetClosestPointOnCollision(GetOwner()->GetActorLocation(), ClosestPointOnCollision);
		RecalculatedSteering = Seek(SteeringVector, ClosestPointOnCollision);
	}
	return RecalculatedSteering;
}


bool UReynoldsSteeringComponent::Separation(FVector & SteeringVector, UObject * WorldContextObject, float VisionDistance, float VisionAngle, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass * ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<class AActor*>& ActorsInRange)
{
	bool RecalculatedSteering = false;
	UKismetSystemLibrary::SphereOverlapActors(WorldContextObject, GetOwner()->GetActorLocation(), VisionDistance, ObjectTypes, ActorClassFilter, ActorsToIgnore, ActorsInRange);
	ActorsInRange.Remove(GetOwner());
	if (ActorsInRange.Num())
	{
		SteeringVector = FVector(0, 0, 0);
		FVector MyLocation = GetOwner()->GetActorLocation();
		for (auto & actor : ActorsInRange)
		{
			if (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetVelocity(), actor->GetActorLocation() - MyLocation))) < VisionAngle)
			{
				FVector ActorLocation = actor->GetActorLocation();
				FVector RepulsiveForce = (MyLocation - ActorLocation).GetSafeNormal() * GetMaxSpeed() / FVector::Dist(MyLocation, ActorLocation);
				SteeringVector += RepulsiveForce;
				RecalculatedSteering = true;
			}
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringComponent::Cohesion(FVector & SteeringVector, UObject * WorldContextObject, float VisionDistance, float VisionAngle, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass * ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<class AActor*>& ActorsInRange)
{
	bool RecalculatedSteering = false;
	UKismetSystemLibrary::SphereOverlapActors(WorldContextObject, GetOwner()->GetActorLocation(), VisionDistance, ObjectTypes, ActorClassFilter, ActorsToIgnore, ActorsInRange);
	ActorsInRange.Remove(GetOwner());
	if (ActorsInRange.GetAllocatedSize())
	{
		FVector MyLocation = GetOwner()->GetActorLocation();
		for (int32_t i = 0;i <ActorsInRange.Num();)
		{
			if (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetVelocity(), ActorsInRange[i]->GetActorLocation() - MyLocation))) >= VisionAngle)
			{
				ActorsInRange.RemoveAt(i);
			}
			else
			{
				i++;
			}
		}
		if (ActorsInRange.Num() > 0)
		{
			FVector AverageDestination = UGameplayStatics::GetActorArrayAverageLocation(ActorsInRange);
			RecalculatedSteering = Seek(SteeringVector, AverageDestination);
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringComponent::Alignment(FVector & SteeringVector, UObject * WorldContextObject, float VisionDistance, float VisionAngle, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass * ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<class AActor*>& ActorsInRange)
{
	bool RecalculatedSteering = false;
	UKismetSystemLibrary::SphereOverlapActors(WorldContextObject, GetOwner()->GetActorLocation(), VisionDistance, ObjectTypes, ActorClassFilter, ActorsToIgnore, ActorsInRange);
	ActorsInRange.Remove(GetOwner());
	if (ActorsInRange.GetAllocatedSize())
	{
		int32_t NumActorsInVision = 0;
		FVector MyLocation = GetOwner()->GetActorLocation();
		FVector DesiredVelocity(0, 0, 0);
		for (auto & actor : ActorsInRange)
		{
			if (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetVelocity(), actor->GetActorLocation() - MyLocation))) < VisionAngle)
			{
				NumActorsInVision++;
				DesiredVelocity += actor->GetVelocity();;
			}
		}
		if (NumActorsInVision != 0)
		{
			DesiredVelocity /= NumActorsInVision;
			SteeringVector = DesiredVelocity - GetVelocity();
			RecalculatedSteering = true;
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringComponent::Flocking(FVector & SteeringVector, UObject * WorldContextObject, float SeparationWeight, float SeparationVisionDistance, float SeparationVisionAngle, float CohesionWeight, float CohesionVisionDistance, float CohesionVisionAngle, float AlignmentWeight, float AlignmentVisionDistance, float AlignmentVisionAngle, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass * ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<class AActor*>& ActorsInRange)
{
	bool RecalculatedSteering = false;
	FVector SeparationSteering(0, 0, 0);
	FVector CohesionSteering(0, 0, 0);
	FVector CohesionDestination(0, 0, 0);
	FVector AlignmentSteering(0, 0, 0);
	RecalculatedSteering |= Separation(SeparationSteering, WorldContextObject, SeparationVisionDistance, SeparationVisionAngle, ObjectTypes, ActorClassFilter, ActorsToIgnore, ActorsInRange);
	RecalculatedSteering |= Cohesion(CohesionSteering, WorldContextObject, CohesionVisionDistance, CohesionVisionAngle, ObjectTypes, ActorClassFilter, ActorsToIgnore, ActorsInRange);
	RecalculatedSteering |= Alignment(AlignmentSteering, WorldContextObject, AlignmentVisionDistance, AlignmentVisionAngle, ObjectTypes, ActorClassFilter, ActorsToIgnore, ActorsInRange);
	if (RecalculatedSteering)
	{
		SteeringVector = ((SeparationSteering.GetSafeNormal() * SeparationWeight + CohesionSteering.GetSafeNormal() * CohesionWeight + AlignmentSteering.GetSafeNormal() * AlignmentWeight)).GetClampedToMaxSize(GetMaxSpeed());
	}
	return RecalculatedSteering;
}



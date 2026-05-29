#include "ReynoldsSteeringBehavior.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Runtime/Engine/Public/WorldCollision.h"
#include "Kismet/GameplayStatics.h"


UReynoldsSteeringBehavior::UReynoldsSteeringBehavior()
{
	
}

UReynoldsSteeringBehavior::~UReynoldsSteeringBehavior()
{
}

UReynoldsSteeringBehavior::UReynoldsSteeringBehavior(AController * Controller) : OwningController(Controller)
{
	OwningPawn = OwningController->GetPawn();
}

bool UReynoldsSteeringBehavior::Seek(FVector & SteeringVector, FVector const & Destination)
{
	if (OwningPawn)
	{
		SteeringVector = (Destination - OwningPawn->GetActorLocation()).GetSafeNormal() * OwningPawn->GetMovementComponent()->GetMaxSpeed() - OwningPawn->GetMovementComponent()->Velocity;
		SteeringVector = SteeringVector.GetClampedToMaxSize(OwningPawn->GetMovementComponent()->GetMaxSpeed());
		return true;
	}
	else
	{
		return false;
	}
}

bool UReynoldsSteeringBehavior::Flee(FVector & SteeringVector, FVector const & FleeDestination)
{
	if (OwningPawn)
	{
		SteeringVector = (OwningPawn->GetActorLocation() - FleeDestination).GetSafeNormal() * OwningPawn->GetMovementComponent()->GetMaxSpeed() - OwningPawn->GetMovementComponent()->Velocity;
		SteeringVector = SteeringVector.GetClampedToMaxSize(OwningPawn->GetMovementComponent()->GetMaxSpeed());
		return true;
	}
	else
	{
		return false;
	}
}

bool UReynoldsSteeringBehavior::Arrive(FVector & SteeringVector, FVector const & Destination, bool & IsWithinArrivalRadius, float ArrivalRadius)
{
	bool RecalculatedSteering = Seek(SteeringVector, Destination);
	if (OwningPawn)
	{
		float Distance = FVector::Dist(Destination, OwningPawn->GetActorLocation());

		if (Distance < ArrivalRadius)
		{
			SteeringVector = SteeringVector * (Distance / ArrivalRadius);
			SteeringVector = SteeringVector.GetClampedToMaxSize(OwningPawn->GetMovementComponent()->GetMaxSpeed());
			IsWithinArrivalRadius = true;
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringBehavior::Pursue(FVector & SteeringVector, AActor const * Evader, float Offset)
{
	bool RecalculatedSteering = false;
	if (OwningPawn && Evader && IsValid(Evader) && Evader->GetRootComponent())
	{
		if (Offset < 0)
		{
			Offset = OwningPawn->GetMovementComponent()->GetMaxSpeed();
		}
		FVector MyLocation = OwningPawn->GetActorLocation();
		FVector TargetLocation = Evader->GetActorLocation();
		float PredictionInterval = FVector::Dist(TargetLocation, MyLocation) / OwningPawn->GetMovementComponent()->GetMaxSpeed();
		FVector PredictedLocation = TargetLocation + Evader->GetVelocity() * PredictionInterval;

		FVector OffsetTargetLocation = TargetLocation + (MyLocation- TargetLocation).GetSafeNormal() * Offset;
		FVector OffsetPredictedLocation = PredictedLocation + (PredictedLocation - TargetLocation).GetSafeNormal() * Offset;

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

bool UReynoldsSteeringBehavior::Evade(FVector & SteeringVector, AActor const * Pursuer)
{
	bool RecalculatedSteering = false;
	if (OwningPawn && Pursuer && IsValid(Pursuer) && Pursuer->GetRootComponent())
	{
		FVector MyLocation = OwningPawn->GetActorLocation();
		FVector TargetLocation = Pursuer->GetActorLocation();
		float PredictionInterval = FVector::Dist(TargetLocation, MyLocation) / OwningPawn->GetMovementComponent()->GetMaxSpeed();
		FVector PredictedLocation = TargetLocation + Pursuer->GetVelocity() * PredictionInterval;

		//This will prevent the evader from evading into the pursuer, but it also lead to flee behavior (rather than evade behavior) far too often.
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

bool UReynoldsSteeringBehavior::Wander(FVector & SteeringVector, float SphereDistance, float SphereRadius)
{
	bool RecalculatedSteering = false;
	if (OwningPawn)
	{
		if (SphereRadius < 0)
		{
			SphereRadius = OwningPawn->GetMovementComponent()->GetMaxSpeed();
		}
		FVector SphereCenterLocation = OwningPawn->GetMovementComponent()->Velocity.GetSafeNormal()*SphereDistance;
		FVector WanderDestination = OwningPawn->GetActorLocation() + SphereCenterLocation + FMath::VRand() * SphereRadius;

		RecalculatedSteering = Seek(SteeringVector, WanderDestination);
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringBehavior::Interpose(FVector & SteeringVector, TArray<AActor*> ActorArray, TArray<float> ActorWeightsArray)
{
	bool RecalculatedSteering = false;
	if (OwningPawn)
	{
		if (ActorWeightsArray.Num() >= 2 && ActorWeightsArray.Num() <= ActorArray.Num())
		{
			float SumOfWeights = 0;
			FVector MyLocation = OwningPawn->GetActorLocation();

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

			float PredictionInterval = FVector::Dist(AverageWeightedActorLocation, MyLocation) / OwningPawn->GetMovementComponent()->GetMaxSpeed();

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
			FVector MyLocation = OwningPawn->GetActorLocation();
			float PredictionInterval = FVector::Dist(AverageActorLocation, MyLocation) / OwningPawn->GetMovementComponent()->GetMaxSpeed();

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
				AveragePredictedLocation /= NumActors;
				RecalculatedSteering = Seek(SteeringVector, AveragePredictedLocation);
			}
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringBehavior::CylindricalObstacleAvoidanceByChannel(FVector & SteeringVector, UObject * WorldContextObject, float LookAheadDistance, float Radius, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult & OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	bool RecalculatedSteering = false;
	if (OwningPawn && OwningPawn->GetMovementComponent()->Velocity.Size() > 0)
	{
		FVector Start = OwningPawn->GetActorLocation() + OwningPawn->GetMovementComponent()->Velocity.GetSafeNormal()*(Radius);
		FVector End = Start + (Start - OwningPawn->GetActorLocation()).GetSafeNormal()* (LookAheadDistance - 2 * Radius);
		if (UKismetSystemLibrary::SphereTraceSingle(WorldContextObject, Start, End, Radius, TraceChannel, bTraceComplex, ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime))
		{
			SteeringVector = OutHit.ImpactNormal*OwningPawn->GetMovementComponent()->GetMaxSpeed()* LookAheadDistance / OutHit.Distance;
			SteeringVector = SteeringVector.GetClampedToMaxSize(OwningPawn->GetMovementComponent()->GetMaxSpeed());
			RecalculatedSteering = true;
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringBehavior::CylindricalObstacleAvoidanceForObjects(FVector & SteeringVector, UObject * WorldContextObject, float LookAheadDistance, float Radius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult & OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	bool RecalculatedSteering = false;
	if (OwningPawn && OwningPawn->GetMovementComponent()->Velocity.Size() > 0)
	{
		FVector Start = OwningPawn->GetActorLocation() + OwningPawn->GetMovementComponent()->Velocity.GetSafeNormal()*(Radius);
		FVector End = Start + ((Start - OwningPawn->GetActorLocation()).GetSafeNormal())*(LookAheadDistance - Radius);
		if (UKismetSystemLibrary::SphereTraceSingleForObjects(WorldContextObject, Start, End, Radius, ObjectTypes, bTraceComplex, ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime))
		{
			SteeringVector = OutHit.Normal*OwningPawn->GetMovementComponent()->GetMaxSpeed()* LookAheadDistance / OutHit.Distance;
			SteeringVector = SteeringVector.GetClampedToMaxSize(OwningPawn->GetMovementComponent()->GetMaxSpeed());
			RecalculatedSteering = true;
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringBehavior::UnalignedCollisionAvoidance(FVector & SteeringVector, float AvoidanceDistance, TArray<AActor*> ActorArray)
{
	bool RecalculatedSteering = false;
	if (OwningPawn)
	{
		for (auto & Actor : ActorArray)
		{
			FVector DifferenceInPositions = OwningPawn->GetActorLocation() - Actor->GetActorLocation();
			FVector DifferenceInVelocity = ((OwningPawn->GetMovementComponent()->Velocity - Actor->GetVelocity()));
			if (DifferenceInVelocity.Size())
			{
				float TimeOfClosestApproach = FVector::DotProduct(-DifferenceInPositions, DifferenceInVelocity) / DifferenceInVelocity.SizeSquared();
				if (TimeOfClosestApproach > 0)
				{
					FVector ThisPositionAtClosestApproach = OwningPawn->GetActorLocation() + OwningPawn->GetMovementComponent()->Velocity*TimeOfClosestApproach;
					FVector OtherPositionAtClosestApproach = Actor->GetActorLocation() + Actor->GetVelocity()*TimeOfClosestApproach;
					if ((OtherPositionAtClosestApproach - ThisPositionAtClosestApproach).Size() < AvoidanceDistance)
					{
						SteeringVector = (ThisPositionAtClosestApproach - OtherPositionAtClosestApproach).GetSafeNormal()*OwningPawn->GetMovementComponent()->GetMaxSpeed();
						SteeringVector = SteeringVector.GetClampedToMaxSize(OwningPawn->GetMovementComponent()->GetMaxSpeed());
						RecalculatedSteering = true;
					}
				}
			}
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringBehavior::FollowPath(FVector & SteeringVector, USplineComponent const * Path, float PathRadius, float LookAheadDistance)
{
	bool RecalculatedSteering = false;
	if (OwningPawn)
	{
		/*
		FVector PredictedLocation = OwningPawn->GetActorLocation() + OwningPawn->GetMovementComponent()->Velocity.GetSafeNormal()*LookAheadDistance;
		Path->Distance
		float InputKeyClosestToPredictedLocation = Path->FindInputKeyClosestToWorldLocation(PredictedLocation);
		FVector LocationOnSplinePlusLookAhead = Path->InputKey
		*/
		FVector PredictedLocation = OwningPawn->GetActorLocation() + OwningPawn->GetMovementComponent()->Velocity.GetSafeNormal()*LookAheadDistance;
		FVector ClosestPointOnPathToPredicted = Path->FindLocationClosestToWorldLocation(PredictedLocation, ESplineCoordinateSpace::World);
		if (((PredictedLocation - ClosestPointOnPathToPredicted).Size() > PathRadius) || (PredictedLocation - ClosestPointOnPathToPredicted).Size() == 0)
		{
			FVector DestinationOnPath = ClosestPointOnPathToPredicted + Path->FindDirectionClosestToWorldLocation(PredictedLocation, ESplineCoordinateSpace::World) * LookAheadDistance;
			RecalculatedSteering = Seek(SteeringVector, DestinationOnPath);
		}
		else
		{
			SteeringVector = Path->FindDirectionClosestToWorldLocation(OwningPawn->GetActorLocation(), ESplineCoordinateSpace::World) * OwningPawn->GetMovementComponent()->GetMaxSpeed();
			RecalculatedSteering = true;
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringBehavior::ContainInActor(FVector & SteeringVector, AActor * Container)
{
	bool RecalculatedSteering = false;
	if (OwningPawn && Container && !Container->IsOverlappingActor(OwningPawn))
	{
		FVector DestinationCandidate(0, 0, 0);
		float ShortestDistance = TNumericLimits<float>::Max();
		float currentDistance = 0;
		TArray<UPrimitiveComponent*> Primitives;
		Container->GetComponents<UPrimitiveComponent>(Primitives);
		if (Primitives.Num())
		{
			for (auto & Primitive : Primitives)
			{
				currentDistance = Primitive->GetClosestPointOnCollision(OwningPawn->GetActorLocation(), DestinationCandidate);
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

bool UReynoldsSteeringBehavior::ContainInPrimitive(FVector & SteeringVector, UPrimitiveComponent * Container)
{
	bool RecalculatedSteering = false;
	if (OwningPawn && !Container->IsOverlappingActor(OwningPawn))
	{
		FVector DestinationCandidate(0, 0, 0);
		float ShortestDistance = TNumericLimits<float>::Max();
		float currentDistance = Container->GetClosestPointOnCollision(OwningPawn->GetActorLocation(), DestinationCandidate);
		RecalculatedSteering = Seek(SteeringVector, DestinationCandidate);
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringBehavior::Separation(FVector & SteeringVector, UObject * WorldContextObject, float VisionDistance, float VisionAngle, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass * ActorClassFilter, const TArray<AActor*>& ActorsToIgnore)
{
	bool RecalculatedSteering = false;
	if (OwningPawn)
	{
		TArray<class AActor*> ActorsInRange = {};
		UKismetSystemLibrary::SphereOverlapActors(WorldContextObject, OwningPawn->GetActorLocation(), VisionDistance, ObjectTypes, ActorClassFilter, ActorsToIgnore, ActorsInRange);
		ActorsInRange.Remove(OwningPawn);
		if (ActorsInRange.Num())
		{
			SteeringVector = FVector(0, 0, 0);
			FVector MyLocation = OwningPawn->GetActorLocation();
			for (auto & actor : ActorsInRange)
			{
				if (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(OwningPawn->GetMovementComponent()->Velocity, actor->GetActorLocation() - MyLocation))) < VisionAngle)
				{
					FVector ActorLocation = actor->GetActorLocation();
					FVector RepulsiveForce = (MyLocation - ActorLocation).GetSafeNormal() * OwningPawn->GetMovementComponent()->GetMaxSpeed() / FVector::Dist(MyLocation, ActorLocation);
					SteeringVector += RepulsiveForce;
					RecalculatedSteering = true;
				}
			}
			SteeringVector = SteeringVector.GetClampedToMaxSize(OwningPawn->GetMovementComponent()->GetMaxSpeed());
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringBehavior::Cohesion(FVector & SteeringVector, UObject * WorldContextObject, float VisionDistance, float VisionAngle, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass * ActorClassFilter, const TArray<AActor*>& ActorsToIgnore)
{
	bool RecalculatedSteering = false;
	if (OwningPawn)
	{
		TArray<class AActor*> ActorsInRange = {};
		UKismetSystemLibrary::SphereOverlapActors(WorldContextObject, OwningPawn->GetActorLocation(), VisionDistance, ObjectTypes, ActorClassFilter, ActorsToIgnore, ActorsInRange);
		ActorsInRange.Remove(OwningPawn);
		if (ActorsInRange.GetAllocatedSize())
		{
			FVector MyLocation = OwningPawn->GetActorLocation();
			for (int32_t i = 0; i < ActorsInRange.Num();)
			{
				if (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(OwningPawn->GetMovementComponent()->Velocity, ActorsInRange[i]->GetActorLocation() - MyLocation))) >= VisionAngle)
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
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringBehavior::Alignment(FVector & SteeringVector, UObject * WorldContextObject, float VisionDistance, float VisionAngle, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass * ActorClassFilter, const TArray<AActor*>& ActorsToIgnore)
{
	bool RecalculatedSteering = false;
	if (OwningPawn)
	{
		TArray<class AActor*> ActorsInRange = {};
		UKismetSystemLibrary::SphereOverlapActors(WorldContextObject, OwningPawn->GetActorLocation(), VisionDistance, ObjectTypes, ActorClassFilter, ActorsToIgnore, ActorsInRange);
		ActorsInRange.Remove(OwningPawn);
		if (ActorsInRange.GetAllocatedSize())
		{
			int32_t NumActorsInVision = 0;
			FVector MyLocation = OwningPawn->GetActorLocation();
			FVector DesiredVelocity(0, 0, 0);
			for (auto & actor : ActorsInRange)
			{
				if (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(OwningPawn->GetMovementComponent()->Velocity, actor->GetActorLocation() - MyLocation))) < VisionAngle)
				{
					NumActorsInVision++;
					DesiredVelocity += actor->GetVelocity();;
				}
			}
			if (NumActorsInVision != 0)
			{
				DesiredVelocity /= NumActorsInVision;
				SteeringVector = DesiredVelocity - OwningPawn->GetMovementComponent()->Velocity;
				SteeringVector = SteeringVector.GetClampedToMaxSize(OwningPawn->GetMovementComponent()->GetMaxSpeed());
				RecalculatedSteering = true;
			}
		}
	}
	return RecalculatedSteering;
}

bool UReynoldsSteeringBehavior::Flocking(FVector & SteeringVector, UObject* WorldContextObject, float AlignmentWeight,float AlignmentVisionDistance, float AlignmentVisionAngle, float CohesionWeight, float CohesionVisionDistance, float CohesionVisionAngle, float SeparationWeight, float SeparationVisionDistance, float SeparationVisionAngle, const TArray < TEnumAsByte < EObjectTypeQuery > > & ObjectTypes, UClass * ActorClassFilter, const TArray < AActor * > & ActorsToIgnore)
{
	bool RecalculatedSteering = false;
	if (OwningPawn)
	{
		FVector AlignmentSteering(0, 0, 0);
		FVector CohesionSteering(0, 0, 0);
		FVector SeparationSteering(0, 0, 0);
		
		RecalculatedSteering |= Alignment(AlignmentSteering, WorldContextObject, AlignmentVisionDistance, AlignmentVisionAngle, ObjectTypes, ActorClassFilter, ActorsToIgnore);
		RecalculatedSteering |= Cohesion(CohesionSteering, WorldContextObject, CohesionVisionDistance, CohesionVisionAngle, ObjectTypes, ActorClassFilter, ActorsToIgnore);
		RecalculatedSteering |= Separation(SeparationSteering, WorldContextObject, SeparationVisionDistance, SeparationVisionAngle, ObjectTypes, ActorClassFilter, ActorsToIgnore);
		
		if (RecalculatedSteering)
		{
			SteeringVector = ((AlignmentSteering.GetSafeNormal() * AlignmentWeight + CohesionSteering.GetSafeNormal() * CohesionWeight + SeparationSteering.GetSafeNormal() * SeparationWeight)).GetClampedToMaxSize(OwningPawn->GetMovementComponent()->GetMaxSpeed());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Pawn for flock"));
	}
	return RecalculatedSteering;
}


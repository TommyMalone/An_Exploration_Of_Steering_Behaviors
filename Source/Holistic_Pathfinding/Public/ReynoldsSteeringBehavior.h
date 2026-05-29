// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Core/Public/Containers/Map.h"
#include "ReynoldsSteeringBehavior.generated.h"


class AController;
class AActor;
class UFloatingPawnMovement;
class USplineComponent;
/**
 * 
 */

UCLASS(ClassGroup = Steering, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class HOLISTIC_PATHFINDING_API UReynoldsSteeringBehavior : public UObject
{
	GENERATED_BODY()
public:
	UReynoldsSteeringBehavior();
	virtual ~UReynoldsSteeringBehavior();
	virtual bool CalculateSteeringVector(FVector & steeringVector) { return false; };

	virtual void Initialize() {};		//Should be called on being play. Currently soley exist for Path Following Behavior becasue it's spline component does not load correctly.
										//This is a bad solution but as a UObject I don't have a begin play call to override and PostLoad(), the only function that looks like it might be what I want
										//has the PathActor as null when it is called, thus you cannot get a spline subobject from it. Currently being called when an AI controller OnPossesses a pawn.

	AController * OwningController = nullptr;
	APawn * OwningPawn = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Parameters")
	float Weight = 1;

protected:
	UReynoldsSteeringBehavior(AController* Controller);
	bool Seek(FVector & SteeringVector, FVector const & Destination);
	bool Flee(FVector & SteeringVector, FVector const & FleeDestination);
	bool Arrive(FVector & SteeringVector, FVector const & Destination, bool & IsWithinArrivalRadius, float ArrivalRadius = 0);
	bool Pursue(FVector & SteeringVector, AActor const * Evader, float Offset = 0);
	bool Evade(FVector & SteeringVector, AActor const * Pursuer);
	bool Wander(FVector & SteeringVector, float SphereDistance = 0, float SphereRadius = -1);
	bool Interpose(FVector & SteeringVector, TArray<AActor*> ActorArray, TArray<float> ActorWeightsArray);
	//Nonconst actor array because UGameplayStatics::GetActorArrayAverageLocation(ActorArray) has no const version.
	bool CylindricalObstacleAvoidanceByChannel(
			FVector & SteeringVector,
			UObject* WorldContextObject,
			float LookAheadDistance,
			float Radius,
			ETraceTypeQuery TraceChannel,
			bool bTraceComplex,
			const TArray<AActor*>& ActorsToIgnore,
			EDrawDebugTrace::Type DrawDebugType,
			FHitResult& OutHit,
			bool bIgnoreSelf,
			FLinearColor TraceColor = FLinearColor::Red,
			FLinearColor TraceHitColor = FLinearColor::Green,
			float DrawTime = 5.0f
		);
	bool CylindricalObstacleAvoidanceForObjects(
			FVector & SteeringVector,
			UObject* WorldContextObject,
			float LookAheadDistance,
			float Radius,
			const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes,
			bool bTraceComplex,
			const TArray<AActor*>& ActorsToIgnore,
			EDrawDebugTrace::Type DrawDebugType,
			FHitResult& OutHit,
			bool bIgnoreSelf,
			FLinearColor TraceColor = FLinearColor::Red,
			FLinearColor TraceHitColor = FLinearColor::Green,
			float DrawTime = 5.0f
		);

	bool UnalignedCollisionAvoidance(FVector & SteeringVector, float AvoidanceDistance, TArray<AActor*> ActorArray);
	bool FollowPath(FVector & SteeringVector, USplineComponent const * Path, float PathRadius, float LookAheadDistance);
	bool ContainInActor(FVector & SteeringVector, AActor * Container);
	bool ContainInPrimitive(FVector & SteeringVector, UPrimitiveComponent * Container);
	bool Separation(
			FVector & SteeringVector,
			UObject* WorldContextObject,
			float VisionDistance,
			float VisionAngle,
			const TArray < TEnumAsByte < EObjectTypeQuery > > & ObjectTypes,
			UClass * ActorClassFilter,
			const TArray < AActor * > & ActorsToIgnore
		);
	bool Cohesion(
			FVector & SteeringVector,
			UObject* WorldContextObject,
			float VisionDistance,
			float VisionAngle,
			const TArray < TEnumAsByte < EObjectTypeQuery > > & ObjectTypes,
			UClass * ActorClassFilter,
			const TArray < AActor * > & ActorsToIgnore
		);
	bool Alignment(
			FVector & SteeringVector,
			UObject* WorldContextObject,
			float VisionDistance,
			float VisionAngle,
			const TArray < TEnumAsByte < EObjectTypeQuery > > & ObjectTypes,
			UClass * ActorClassFilter,
			const TArray < AActor * > & ActorsToIgnore
		);
	bool Flocking(
			FVector & SteeringVector,
			UObject* WorldContextObject,
			float AlignmentWeight,
			float AlignmentVisionDistance,
			float AlignmentVisionAngle,
			float CohesionWeight,
			float CohesionVisionDistance,
			float CohesionVisionAngle,
			float SeparationWeight,
			float SeparationVisionDistance,
			float SeparationVisionAngle,
			
			const TArray < TEnumAsByte < EObjectTypeQuery > > & ObjectTypes,
			UClass * ActorClassFilter,
			const TArray < AActor * > & ActorsToIgnore
		);
};

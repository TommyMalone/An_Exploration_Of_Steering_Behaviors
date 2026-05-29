// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Core/Public/Containers/Map.h"
#include "ReynoldsSteeringComponent.generated.h"


class AActor;
class UFloatingPawnMovement;
class USplineComponent;
//struct FCollisionShape;
//struct FCollisionQueryParams;
//struct FCollisionResponseParams;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLISTIC_PATHFINDING_API UReynoldsSteeringComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReynoldsSteeringComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual UFloatingPawnMovement const * GetMovementComponent() const;
	virtual void SetMovementComponent(UFloatingPawnMovement* Component);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	FVector GetVelocity();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	float GetMaxSpeed();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	float GetAcceleration();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	float GetDeceleration();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	float GetTurnBoost();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	FVector ClampSteeringToMaxSpeed(FVector const & SteeringVector);
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	FVector ScaleSteeringToInputVector(FVector const & ScaledSteeringVector);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	bool Seek(FVector & SteeringVector, FVector const & Destination);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	bool Flee(FVector & SteeringVector, FVector const & FleeDestination);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	bool Arrive(FVector & SteeringVector, FVector const & Destination, bool & IsWithinArrivalRadius, float ArrivalRadius = -1);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	bool Pursue(FVector & SteeringVector, AActor const * Evader, float Offset = 0);
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	bool Evade(FVector & SteeringVector, AActor const * Pursuer);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	bool Wander(FVector & SteeringVector, float SphereDistance = 0, float SphereRadius = -1);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "", AutoCreateRefTerm = "ActorWeightsArray"), Category = Steering)
	bool Interpose(FVector & SteeringVector, TArray<AActor*> ActorArray, TArray<float> ActorWeightsArray);
	//Nonconst actor array because UGameplayStatics::GetActorArrayAverageLocation(ActorArray) has no const version.

	UFUNCTION(BlueprintCallable, meta=(bIgnoreSelf="true", WorldContext="WorldContextObject", AutoCreateRefTerm="ActorsToIgnore", AdvancedDisplay="TraceColor,TraceHitColor,DrawTime", Keywords="sweep", Tooltip = ""), Category = Steering)
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

	UFUNCTION(BlueprintCallable, meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceColor,TraceHitColor,DrawTime", Keywords = "sweep", Tooltip = ""), Category = Steering)
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

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	bool UnalignedCollisionAvoidance(FVector & SteeringVector, float AvoidanceDistance, TArray<AActor*> ActorArray);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	bool FollowPath(FVector & SteeringVector, USplineComponent const * Path, float PathRadius, float LookAheadDistance);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = Steering)
	bool FollowWall(FVector & SteeringVector);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Make sure both actors generate overlap events. Component must have simple collision."), Category = Steering)
	bool ContainInActor(FVector & SteeringVector, AActor * Container);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Make sure both actors generate overlap events. Component must have simple collision."), Category = Steering)
	bool ContainInPrimitive(FVector & SteeringVector, UPrimitiveComponent * Container);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", Tooltip = ""), Category = Steering)
	bool Separation(
		FVector & SteeringVector,
		UObject* WorldContextObject,
		float VisionDistance,
		float VisionAngle,
		const TArray < TEnumAsByte < EObjectTypeQuery > > & ObjectTypes,
		UClass * ActorClassFilter,
		const TArray < AActor * > & ActorsToIgnore,
		TArray < class AActor * > & ActorsInRange
	);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", Tooltip = ""), Category = Steering)
	bool Cohesion(
		FVector & SteeringVector,
		UObject* WorldContextObject,
		float VisionDistance,
		float VisionAngle,
		const TArray < TEnumAsByte < EObjectTypeQuery > > & ObjectTypes,
		UClass * ActorClassFilter,
		const TArray < AActor * > & ActorsToIgnore,
		TArray < class AActor * > & ActorsInRange
	);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", Tooltip = ""), Category = Steering)
	bool Alignment(
		FVector & SteeringVector,
		UObject* WorldContextObject,
		float VisionDistance,
		float VisionAngle,
		const TArray < TEnumAsByte < EObjectTypeQuery > > & ObjectTypes,
		UClass * ActorClassFilter,
		const TArray < AActor * > & ActorsToIgnore,
		TArray < class AActor * > & ActorsInRange
	);

	//AdvancedDisplay = "SeparationVisionDistance, SeparationVisionAngle, CohesionVisionDistance, CohesionVisionAngle, AlignmentVisionDistance, AlignmentVisionAngle",
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", Tooltip = ""), Category = Steering)
	bool Flocking(
		FVector & SteeringVector,
		UObject* WorldContextObject,
		float SeparationWeight,
		float SeparationVisionDistance,
		float SeparationVisionAngle,
		float CohesionWeight,
		float CohesionVisionDistance,
		float CohesionVisionAngle,
		float AlignmentWeight,
		float AlignmentVisionDistance,
		float AlignmentVisionAngle,
		const TArray < TEnumAsByte < EObjectTypeQuery > > & ObjectTypes,
		UClass * ActorClassFilter,
		const TArray < AActor * > & ActorsToIgnore,
		TArray < class AActor * > & ActorsInRange
	);

protected:
	UFloatingPawnMovement * MovementComponent = nullptr;
};

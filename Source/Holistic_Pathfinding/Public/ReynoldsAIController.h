// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ReynoldsSteeringBehavior.h"
#include "RestSteeringBehavior.h"
#include "SeekSteeringBehavior.h"
#include "FleeSteeringBehavior.h"
#include "ArriveSteeringBehavior.h"
#include "PursueSteeringBehavior.h"
#include "EvadeSteeringBehavior.h"
#include "WanderSteeringBehavior.h"
#include "InterposeSteeringBehavior.h"
#include "AvoidObstaclesSteeringBehavior.h"
#include "AvoidCollisionsSteeringBehavior.h"
#include "PathFollowSteeringBehavior.h"
#include "ContainmentSteeringBehavior.h"
#include "SeparationSteeringBehavior.h"
#include "AlignmentSteeringBehavior.h"
#include "CohesionSteeringBehavior.h"
#include "FlockingSteeringBehavior.h"
#include "ReynoldsAIController.generated.h"


UENUM(BlueprintType)
enum class ESteeringBehavior : uint8
{
	Rest,
	Seek,
	Flee,
	Arrive,
	Pursue,
	Evade,
	Wander,
	Interpose,
	AvoidObstacles,
	AvoidCollisions,
	PathFollow,
	Containment,
	Separation,
	Alignment,
	Cohesion,
	Flocking,
	ENDENUM
};
/**
 * 
 */
UCLASS()
class HOLISTIC_PATHFINDING_API AReynoldsAIController : public AAIController
{
	GENERATED_BODY()

public:
	AReynoldsAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Instanced, Category = "Steering")
	TArray<UReynoldsSteeringBehavior*> SteeringBehaviors;
	UPROPERTY(BlueprintReadWrite, Category = "Steering")
	TArray< UReynoldsSteeringBehavior*> CurrentSteeringBehavior;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Steering")
	FVector CurrentSteeringVector = FVector(0,0,0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Movement")
	bool bOverrideMovementComponentSettings = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Movement")
	float  MaxSpeed = 1200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Movement")
	float  Acceleration = 4000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Movement")
	float  Decceleration = 8000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Movement")
	float  TurningBoost = 8;

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	URestSteeringBehavior* GetRestBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	USeekSteeringBehavior* GetSeekBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	UFleeSteeringBehavior* GetFleeBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	UArriveSteeringBehavior* GetArriveBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	UPursueSteeringBehavior* GetPursueBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	UEvadeSteeringBehavior* GetEvadeBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	UWanderSteeringBehavior* GetWanderBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	UInterposeSteeringBehavior* GetInterposeBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	UAvoidObstaclesSteeringBehavior* GetAvoidObstaclesBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	UAvoidCollisionsSteeringBehavior* GetAvoidCollisionsBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	UPathFollowSteeringBehavior* GetPathFollowBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	UContainmentSteeringBehavior* GetContainmentBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	USeparationSteeringBehavior* GetSeparationBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	UAlignmentSteeringBehavior* GetAlignmentBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	UCohesionSteeringBehavior* GetCohesionBehavior();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	UFlockingSteeringBehavior* GetFlockingBehavior();
	
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	void AddBehaviorToCurrentSteeringBehavior(UReynoldsSteeringBehavior* newBehavior);
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	void ClearCurrentSteeringBehaviors();
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	FVector CalculateCurrentSteeringVector();
	
	


};

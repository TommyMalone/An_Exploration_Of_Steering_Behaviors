// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReynoldsSteeringBehavior.h"
#include "FlockingSteeringBehavior.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Steering, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class HOLISTIC_PATHFINDING_API UFlockingSteeringBehavior : public UReynoldsSteeringBehavior
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	bool CalculateSteeringVector(FVector & SteeringVector) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float AlignmentWeight = 40;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float AlignmentVisionDistance = 800;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float AlignmentVisionAngle = 360;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float CohesionWeight = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float CohesionVisionDistance = 800;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float CohesionVisionAngle = 360;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float SeparationWeight = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float SeparationVisionDistance = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float SeparationVisionAngle = 360;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	UClass * ActorClassFilter = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	TArray<AActor*> ActorsToIgnore = {};
	
	
};

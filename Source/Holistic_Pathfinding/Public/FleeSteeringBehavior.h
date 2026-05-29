// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReynoldsSteeringBehavior.h"
#include "FleeSteeringBehavior.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Steering, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class HOLISTIC_PATHFINDING_API UFleeSteeringBehavior : public UReynoldsSteeringBehavior
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	bool CalculateSteeringVector(FVector & SteeringVector) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	AActor * FleeTarget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	FVector FleeDestination = FVector(0, 0, 0);
	
};

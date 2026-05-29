// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReynoldsSteeringBehavior.h"
#include "SeekSteeringBehavior.generated.h"

/**
 * 
 */

UCLASS(ClassGroup = Steering, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class HOLISTIC_PATHFINDING_API USeekSteeringBehavior final: public UReynoldsSteeringBehavior
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	bool CalculateSteeringVector(FVector & SteeringVector) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	AActor* SeekTarget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	FVector Destination = FVector(0,0,0);
	
};

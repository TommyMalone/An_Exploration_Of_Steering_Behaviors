// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReynoldsSteeringBehavior.h"
#include "InterposeSteeringBehavior.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Steering, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class HOLISTIC_PATHFINDING_API UInterposeSteeringBehavior : public UReynoldsSteeringBehavior
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	bool CalculateSteeringVector(FVector & SteeringVector) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	TArray<AActor *> InterposeTargets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	TArray<float> OptionalTargetWeights;
};

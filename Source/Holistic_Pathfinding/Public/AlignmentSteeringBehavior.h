// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReynoldsSteeringBehavior.h"
#include "AlignmentSteeringBehavior.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Steering, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class HOLISTIC_PATHFINDING_API UAlignmentSteeringBehavior : public UReynoldsSteeringBehavior
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	bool CalculateSteeringVector(FVector & SteeringVector) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float VisionDistance = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float VisionAngle = 360;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	UClass * ActorClassFilter = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	TArray<AActor*> ActorsToIgnore = {};

};

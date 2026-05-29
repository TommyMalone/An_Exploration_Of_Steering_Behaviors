// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReynoldsSteeringBehavior.h"
#include "ArriveSteeringBehavior.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Steering, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class HOLISTIC_PATHFINDING_API UArriveSteeringBehavior : public UReynoldsSteeringBehavior
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	bool CalculateSteeringVector(FVector & SteeringVector) override;
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	bool IsWithinArrivalRadius();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	AActor* ArriveTarget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	FVector Destination = FVector(0, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float ArrivalRadius = 0;
private:
	bool bIsWithinArrivalRadius = false;
	
};

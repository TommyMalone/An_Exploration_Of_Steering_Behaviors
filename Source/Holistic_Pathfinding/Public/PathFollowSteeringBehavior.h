// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReynoldsSteeringBehavior.h"
#include "PathFollowSteeringBehavior.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Steering, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class HOLISTIC_PATHFINDING_API UPathFollowSteeringBehavior : public UReynoldsSteeringBehavior
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	bool CalculateSteeringVector(FVector & SteeringVector) override;
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	bool IsOnPath();

	virtual void Initialize();

	virtual void PostLoad(); //override

	virtual void PostInitProperties(); // override

	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent); // override

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	AActor * PathActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	FName SplineComponentName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	USplineComponent * Path = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float PathRadius = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float LookAheadDistance = 0;
	
	
};

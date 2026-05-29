// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReynoldsSteeringBehavior.h"
#include "AvoidObstaclesSteeringBehavior.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ETraceType : uint8
{
	Channel,
	Object,
	ENDENUM
};

UCLASS(ClassGroup = Steering, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class HOLISTIC_PATHFINDING_API UAvoidObstaclesSteeringBehavior : public UReynoldsSteeringBehavior
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "Steering")
	bool CalculateSteeringVector(FVector & SteeringVector) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	ETraceType TraceType = ETraceType::Object;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float LookAheadDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	TEnumAsByte<ETraceTypeQuery> ChannelTypes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	bool bTraceComplex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	TArray<AActor*> ActorsToIgnore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;
	
	UPROPERTY(BlueprintReadOnly, Category = "Steering Parameters")
	FHitResult OutHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	bool bIgnoreSelf;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	FLinearColor TraceColor = FLinearColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	FLinearColor TraceHitColor = FLinearColor::Green;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Parameters")
	float DrawTime = 5.0f;
};

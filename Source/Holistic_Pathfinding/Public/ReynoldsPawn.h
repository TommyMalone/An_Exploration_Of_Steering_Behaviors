// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ReynoldsSteeringBehavior.h"
//#include "Runtime/AIModule/Classes/AIController.h"
#include "ReynoldsAIController.h"
#include "ReynoldsPawn.generated.h"

class UArrowComponent;
class UFloatingPawnMovement;
class UReynoldsSteeringComponent;

UCLASS()
class HOLISTIC_PATHFINDING_API AReynoldsPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AReynoldsPawn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<TSubclassOf<AReynoldsAIController>> AIControllerTypesList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Instanced, Category = "AI")
	TArray <AReynoldsAIController*> AIControllersList;

private:

	UPROPERTY(Category = ReynoldsPawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UFloatingPawnMovement * FloatingPawnMovementComponent;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing */
	UPROPERTY()
		UArrowComponent* ArrowComponent;
#endif
public:

	 /// <summary>
	 /// 
	 /// </summary>
	 /// <returns></returns>
	UFloatingPawnMovement* GetMovement() const { return FloatingPawnMovementComponent; }

	/** Name of the CharacterMovement component. Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	/// <summary>
	/// 
	/// </summary>
	static FName FloatingPawnMovementComponentName;

#if WITH_EDITORONLY_DATA
	/** Returns ArrowComponent subobject **/
	class UArrowComponent* GetArrowComponent() const { return ArrowComponent; }
#endif
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void OnConstruction(const FTransform& Transform); // override
	
#if WITH_EDITOR
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Keeps one on-screen line per AI controller class in use, each controller being one demo behavior. A line stays while any boid runs that behavior.
	virtual void NotifyControllerChanged() override;

	
	
};

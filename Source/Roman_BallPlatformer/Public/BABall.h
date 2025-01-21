// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputSubsystems.h" // Include for Enhanced Input
#include "EnhancedInputComponent.h" // Include for Enhanced Input Component
#include "InputMappingContext.h"    // Include for Input Mapping Context
#include "InputAction.h"  
#include "BABall.generated.h"

UCLASS()
class ROMAN_BALLPLATFORMER_API ABABall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABABall();

private:
	UPROPERTY(VisibleAnywhere, Category = "Move")
	UStaticMeshComponent* BallComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	float JumpForce;

	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	//UPROPERTY(EditAnywhere, Category = "Input")
	//TSoftObjectPtr<UInputAction> JumpAction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Jump();
	void Move();
	bool CheckValidity();
};

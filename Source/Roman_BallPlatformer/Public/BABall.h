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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float JumpForce;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IAMove;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IAJump;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Jump(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	bool CheckValidity();
};

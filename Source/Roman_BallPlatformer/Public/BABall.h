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

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	UStaticMeshComponent* BallComponent;

private:

	bool Grounded;
	bool CanMove = true;
	bool IsAttached;
	int JumpCount = 0;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	USceneComponent* Pivot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float JumpForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float DashStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float DownForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float CamRotSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float GroundCheckSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	int MaxJumpCount = 2;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IAMove;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IAJump;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IALook;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IADash;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IADown;

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
	void DashPressed(const FInputActionValue& Value);
	void DashReleased(const FInputActionValue& Value);
	void DownPressed(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void GetGround();
	bool CheckValidity();
};

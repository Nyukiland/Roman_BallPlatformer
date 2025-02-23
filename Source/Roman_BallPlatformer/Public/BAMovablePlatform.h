// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BAMovablePlatform.generated.h"

UCLASS()
class ROMAN_BALLPLATFORMER_API ABAMovablePlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABAMovablePlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	FVector MovementValue;

private:
	int PointIndex;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	UStaticMeshComponent* Platform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	TArray<FVector> Points;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override { return true; }

	void DrawDebugCross(const FVector Location, const FColor Color);

	void MovePlatform(float DeltaTime);
};

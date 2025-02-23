// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BABumper.generated.h"

UCLASS()
class ROMAN_BALLPLATFORMER_API ABABumper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABABumper();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Area;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

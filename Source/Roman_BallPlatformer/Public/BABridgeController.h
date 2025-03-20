// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BABridgeConnector.h"
#include "BAPlank.h"
#include "BABridgeController.generated.h"

UCLASS()
class ROMAN_BALLPLATFORMER_API ABABridgeController : public AActor
{
	GENERATED_BODY()
	
public:	
	ABABridgeController();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	TArray<ABABridgeConnector*> Connectors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	TArray<ABAPlank*> Planks;

protected:
	virtual void BeginPlay() override;

private:	
	virtual void Tick(float DeltaTime) override;
	void RegisterConnector(ABABridgeConnector* Connector);
	void RegisterBeam(ABAPlank* Beam);

};

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
	USceneComponent* PointA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	USceneComponent* PointB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	int BridgePlanksCount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	float MaxStress = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	float PullingStrength = 5000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	float MinDistConnector = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	TSubclassOf<ABAPlank> PlankToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	TArray<ABABridgeConnector*> Connectors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	TArray<ABAPlank*> Planks;

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Bridge")
	void GenerateBridge();

protected:	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};

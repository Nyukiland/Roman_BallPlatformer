#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BABridgeConnector.generated.h"

class ABAPlank;

UCLASS()
class ROMAN_BALLPLATFORMER_API ABABridgeConnector : public AActor
{
	GENERATED_BODY()
	
public:	
	ABABridgeConnector();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	bool IsAnchor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	float PullingStrength = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	float MaxPullDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	TArray<ABAPlank*> ConnectedPlank;

public:	
	virtual void Tick(float DeltaTime) override;
	void ApplyPullingForce();
	void ApplyWeight(float Weight);
};

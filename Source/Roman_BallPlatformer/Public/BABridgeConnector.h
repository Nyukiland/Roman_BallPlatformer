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
	float MinPullDistance = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bridge")
	TArray<ABAPlank*> ConnectedPlank;

public:	
	void TickConnector(float DeltaTime);
	void ApplyPullingForce();
};

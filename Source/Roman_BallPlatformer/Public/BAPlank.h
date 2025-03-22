#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BAPlank.generated.h"

class ABABridgeConnector;

UCLASS()
class ROMAN_BALLPLATFORMER_API ABAPlank : public AActor
{
	GENERATED_BODY()
	
public:	
	ABAPlank();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	UStaticMeshComponent* PlankMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	float MaxStress = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	float CurrentStress = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	UMaterialInterface* Mat01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	TArray<ABABridgeConnector*> ConnectedJoints;

private:
	UMaterialInstanceDynamic* DynamicMaterial;
	FVector CurrentExternalForce;
	FVector PreviousExternalForce;

public:	
	virtual void BeginPlay() override;
	void TickPlank(float DeltaTime);
	void ApplyForce(FVector Force);

private:
	float GetStress01() const;
	void UpdateStrength();
	void CheckDestroy();
};

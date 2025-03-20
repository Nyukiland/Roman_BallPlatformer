#include "BAPlank.h"
#include "BABridgeConnector.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

ABAPlank::ABAPlank()
{
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	PlankMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plank"));
	RootComponent = PlankMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded())
	{
		PlankMesh->SetStaticMesh(CubeAsset.Object);
	}

	PlankMesh->SetSimulatePhysics(true);
	PlankMesh->SetLinearDamping(0.1f);
	PlankMesh->SetAngularDamping(0.1f);
}

void ABAPlank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShouldBreak())
	{
		for (ABABridgeConnector* Connector : ConnectedJoints)
		{
			if (Connector)
			{
				Connector->ConnectedPlank.Remove(this);
			}
		}
		Destroy();
	}
}

void ABAPlank::ApplyForce(FVector Force)
{
	PlankMesh->AddForce(Force, NAME_None, true);
}

bool ABAPlank::ShouldBreak() const
{
	return CurrentStress > MaxStress;
}
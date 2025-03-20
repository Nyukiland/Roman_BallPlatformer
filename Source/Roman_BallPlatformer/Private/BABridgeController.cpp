#include "BABridgeController.h"

ABABridgeController::ABABridgeController()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ABABridgeController::BeginPlay()
{
	Super::BeginPlay();
}

void ABABridgeController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (ABABridgeConnector* Connector : Connectors)
	{
		if (Connector)
		{
			Connector->ApplyWeight(100.0f);
		}
	}
}

void ABABridgeController::RegisterConnector(ABABridgeConnector* Connector)
{
	if (Connector)
	{
		Connectors.Add(Connector);
	}
}

void ABABridgeController::RegisterBeam(ABAPlank* Plank)
{
	if (Plank)
	{
		Planks.Add(Plank);
	}
}
#include "BABridgeController.h"

ABABridgeController::ABABridgeController()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	PointA = CreateDefaultSubobject<USceneComponent>(TEXT("PointA"));
	PointA->SetupAttachment(RootComponent);

	PointB = CreateDefaultSubobject<USceneComponent>(TEXT("PointB"));
	PointB->SetupAttachment(RootComponent);
}

void ABABridgeController::GenerateBridge()
{
	if (Connectors.Num() != 0 || Planks.Num() != 0)
	{
		// Delete each connector and plank, assuming that the actors are properly destroyed.
		for (ABABridgeConnector* Connector : Connectors)
		{
			if (Connector)
			{
				Connector->Destroy();
			}
		}
		Connectors.Empty();

		for (ABAPlank* Plank : Planks)
		{
			if (Plank)
			{
				Plank->Destroy();
			}
		}
		Planks.Empty();
	}

	// Get the locations of PointA and PointB
	FVector StartLocation = PointA->GetComponentLocation();
	FVector EndLocation = PointB->GetComponentLocation();

	FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
	float PlankLength = (EndLocation - StartLocation).Size() / BridgePlanksCount;

	//get distance and divide it by the number of BridgePlanksCount
	//for number of BridgePlanksCount create a plank with dir normalised multiplied by the rotation
	//add plank to the list Planks

	//loop on created plank and spawn connector between two planks
	//connect planks to connector and connector to planks using their list

	//create the connector at start and end location and set it to isAnchor

}

void ABABridgeController::BeginPlay()
{
	Super::BeginPlay();

	if (Connectors.Num() == 0 || Planks.Num() == 0)
	{
		GenerateBridge();
	}
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
#include "BABridgeController.h"
#include "Kismet/KismetMathLibrary.h"

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

	ABABridgeConnector* CurConnector = GetWorld()->SpawnActor<ABABridgeConnector>();
	CurConnector->SetActorLocation(StartLocation);
	CurConnector->IsAnchor = true;
	Connectors.Add(CurConnector);

	for (int i = 0; i < BridgePlanksCount; i++)
	{
		ABAPlank* CurPlank = GetWorld()->SpawnActor<ABAPlank>(PlankToSpawn);
		FVector CurPos = StartLocation + Direction * PlankLength * i;
		CurPlank->SetActorLocation(CurPos);
		CurPlank->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CurConnector->GetActorLocation(), CurConnector->GetActorLocation() + Direction));
		Planks.Add(CurPlank);

		CurPlank->ConnectedJoints.Add(CurConnector);

		CurConnector = GetWorld()->SpawnActor<ABABridgeConnector>();
		CurConnector->SetActorLocation(CurPos);
		Connectors.Add(CurConnector);

		if (Planks.Num() != 0) Planks.Last()->ConnectedJoints.Add(CurConnector);
	}

	CurConnector->SetActorLocation(EndLocation);
	CurConnector->IsAnchor = true;
}

void ABABridgeController::BeginPlay()
{
	Super::BeginPlay();

	for (ABABridgeConnector* Connector : Connectors)
	{
		if (Connector)
		{
			Connector->MinPullDistance = MinDistConnector;
			Connector->Stiffness = Stiffness;
			if (Connector->IsAnchor) Connector->PullingStrength = AnchorPullingStrength;
			else Connector->PullingStrength = PullingStrength;
		}
	}

	for (ABAPlank* Plank : Planks)
	{
		if (Plank)
		{
			Plank->MaxStress = MaxStress;
		}
	}
}

void ABABridgeController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (ABABridgeConnector* Connector : Connectors) if (Connector) Connector->TickConnector(DeltaTime);
	for (ABAPlank* Plank : Planks) if (Plank) Plank->TickPlank(DeltaTime);
}
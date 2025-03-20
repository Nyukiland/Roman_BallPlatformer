#include "BABridgeConnector.h"
#include "BAPlank.h"

ABABridgeConnector::ABABridgeConnector()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ABABridgeConnector::Tick(float DeltaTime)
{
	ApplyPullingForce();

	if (!IsAnchor)
	{
		if (ConnectedPlank.Num() > 0)
		{
			FVector MidPos = FVector::ZeroVector;

			for (ABAPlank* Plank : ConnectedPlank)
			{
				if (Plank) MidPos += Plank->GetActorLocation();
			}

			MidPos /= ConnectedPlank.Num();

			SetActorLocation(MidPos);
		}
	}
}

void ABABridgeConnector::ApplyWeight(float Weight)
{
	if (ConnectedPlank.Num() == 0)
		return;

	float ForcePerPlank = Weight / ConnectedPlank.Num();

	for (ABAPlank* Plank : ConnectedPlank)
	{
		if (Plank)
		{
			Plank->ApplyForce(FVector(0, 0, -ForcePerPlank)); // Apply downward force
		}
	}
}

void ABABridgeConnector::ApplyPullingForce()
{
	for (ABAPlank* Plank : ConnectedPlank)
	{
		if (Plank)
		{
			FVector DirectionToConnector = GetActorLocation() - Plank->GetActorLocation();

			float Distance = DirectionToConnector.Size();

			if (Distance > MaxPullDistance)
			{
				continue;
			}

			DirectionToConnector.Normalize();

			float ForceMagnitude = FMath::Clamp(PullingStrength * (MaxPullDistance - Distance), 0.0f, PullingStrength);

			FVector Force = DirectionToConnector * ForceMagnitude;

			Plank->ApplyForce(Force);
		}
	}
}
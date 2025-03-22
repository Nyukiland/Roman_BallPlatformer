#include "BABridgeConnector.h"
#include "BAPlank.h"

ABABridgeConnector::ABABridgeConnector()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ABABridgeConnector::TickConnector(float DeltaTime)
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

void ABABridgeConnector::ApplyPullingForce()
{
	for (ABAPlank* Plank : ConnectedPlank)
	{
		if (Plank)
		{
			FVector DirectionToConnector = GetActorLocation() - Plank->PlankMesh->GetComponentLocation();
			float CurrentDistance = DirectionToConnector.Size();

			float StretchFactor = FMath::Clamp(CurrentDistance - MinPullDistance, -Stiffness, Stiffness);
			float ForceMagnitude = PullingStrength * StretchFactor;

			DirectionToConnector.Normalize();
			FVector Force = DirectionToConnector * ForceMagnitude;

			Plank->ApplyForce(Force);
		}
	}
}
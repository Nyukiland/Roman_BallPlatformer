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
			FVector DirectionToConnector = GetActorLocation() - Plank->GetActorLocation();

			float Distance = DirectionToConnector.Size();
			Distance = FMath::Clamp(Distance, MinPullDistance, Distance);

			DirectionToConnector.Normalize();

			float ForceMagnitude = PullingStrength * (Distance - MinPullDistance);
			
			FVector Force = DirectionToConnector * ForceMagnitude;

			Plank->ApplyForce(Force);
		}
	}
}
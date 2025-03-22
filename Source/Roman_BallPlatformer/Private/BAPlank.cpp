#include "BAPlank.h"
#include "BABridgeConnector.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

ABAPlank::ABAPlank()
{
	PlankMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plank"));
	RootComponent = PlankMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded())
	{
		PlankMesh->SetStaticMesh(CubeAsset.Object);
	}

	PlankMesh->SetSimulatePhysics(true);
	PlankMesh->SetEnableGravity(true);
	PlankMesh->SetLinearDamping(0.1f);
	PlankMesh->SetAngularDamping(0.1f);
	PlankMesh->BodyInstance.bLockXRotation = true;
	PlankMesh->BodyInstance.bLockYRotation = false;
	PlankMesh->BodyInstance.bLockZRotation = true;
}

void ABAPlank::BeginPlay()
{
	Super::BeginPlay();

	if (Mat01)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Mat01, this);
		PlankMesh->SetMaterial(0, DynamicMaterial);
	}

	for (ABABridgeConnector* connect : ConnectedJoints)
	{
		connect->ConnectedPlank.Add(this);
	}
}

void ABAPlank::TickPlank(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateStrength();

	CheckDestroy(DeltaTime);

	if (DynamicMaterial) DynamicMaterial->SetScalarParameterValue(FName("Value"), GetStress01());
}

void ABAPlank::ApplyForce(FVector Force)
{
	CurrentExternalForce += Force;
}

float ABAPlank::GetStress01() const
{
	return FMath::Clamp(CurrentStress / MaxStress, 0, 1);
}

void ABAPlank::UpdateStrength()
{
	PlankMesh->AddForce(CurrentExternalForce, NAME_None, true);

	CurrentStress = CurrentExternalForce.Size();

	CurrentExternalForce = FVector::Zero();
}

void ABAPlank::CheckDestroy(float DeltaTime)
{
	if (GetStress01() >= 1)
	{
		TimerDestroy += DeltaTime;
		if (TimerDestroy > 2)
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
	else TimerDestroy = 0;
}
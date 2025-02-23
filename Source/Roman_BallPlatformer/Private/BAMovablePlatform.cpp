// Fill out your copyright notice in the Description page of Project Settings.


#include "BAMovablePlatform.h"

// Sets default values
ABAMovablePlatform::ABAMovablePlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	RootComponent = Platform;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded())
	{
		Platform->SetStaticMesh(CubeAsset.Object);
	}

	Platform->SetSimulatePhysics(false);
	Platform->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called when the game starts or when spawned
void ABAMovablePlatform::BeginPlay()
{
	Super::BeginPlay();

	if (Points.Num() != 0)
	{
		Platform->SetWorldLocation(Points[0]);
		PointIndex = 0;
	}
}

// Called every frame
void ABAMovablePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GIsEditor && GetWorld()->WorldType == EWorldType::Editor)
	{
		for (FVector Point : Points)
		{
			DrawDebugCross(Point, FColor::Green);
		}
	}
	else
	{
		MovePlatform(DeltaTime);
	}
}

void ABAMovablePlatform::DrawDebugCross(const FVector Location, const FColor Color)
{
	float Size = 50;

	DrawDebugLine(GetWorld(), Location + FVector::UpVector * Size, Location + FVector::DownVector * Size, Color);
	DrawDebugLine(GetWorld(), Location + FVector::ForwardVector * Size, Location + FVector::BackwardVector * Size, Color);
	DrawDebugLine(GetWorld(), Location + FVector::RightVector * Size, Location + FVector::LeftVector * Size, Color);
}

void ABAMovablePlatform::MovePlatform(float DeltaTime)
{
	if (!Platform || Points.Num() == 0) return;

	FVector CurrentLocation = GetActorLocation();
	MovementValue = (Points[PointIndex] - CurrentLocation).GetSafeNormal();

	FVector NewLocation = CurrentLocation + (MovementValue * Speed * DeltaTime);

	if (FVector::Dist(CurrentLocation, Points[PointIndex]) < 1)
	{
		NewLocation = Points[PointIndex];
		PointIndex++;
		if (Points.Num() <= PointIndex) PointIndex = 0;
	}

	SetActorLocation(NewLocation);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "BAJumpPad.h"
#include "BABall.h"

// Sets default values
ABAJumpPad::ABAJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Area = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Area"));
	RootComponent = Area;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded())
	{
		Area->SetStaticMesh(CubeAsset.Object);
	}

	Area->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Area->SetCollisionObjectType(ECC_WorldDynamic);
	Area->SetCollisionResponseToAllChannels(ECR_Overlap);

	Area->OnComponentBeginOverlap.AddDynamic(this, &ABAJumpPad::OnPlayerEnter);
}

void ABAJumpPad::OnPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	ABABall* Ball = Cast<ABABall>(OtherActor);
	if (Ball) Ball->BallComponent->AddImpulse(Area->GetUpVector() * PropultionForce, NAME_None, true);
}
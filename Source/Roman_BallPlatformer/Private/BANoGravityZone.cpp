// Fill out your copyright notice in the Description page of Project Settings.


#include "BANoGravityZone.h"
#include "BABall.h"

// Sets default values
ABANoGravityZone::ABANoGravityZone()
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
	Area->BodyInstance.SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	Area->OnComponentBeginOverlap.AddDynamic(this, &ABANoGravityZone::OnPlayerEnter);
	Area->OnComponentEndOverlap.AddDynamic(this, &ABANoGravityZone::OnPlayerExit);
}

void ABANoGravityZone::OnPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	ABABall* Ball = Cast<ABABall>(OtherActor);
	if (Ball) Ball->BallComponent->SetEnableGravity(false);
}

void ABANoGravityZone::OnPlayerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;

	ABABall* Ball = Cast<ABABall>(OtherActor);
	if (Ball) Ball->BallComponent->SetEnableGravity(true);
}


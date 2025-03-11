// Fill out your copyright notice in the Description page of Project Settings.


#include "BABumper.h"
#include "BABall.h"

// Sets default values
ABABumper::ABABumper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Area = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Area"));
	RootComponent = Area;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylinderAsset.Succeeded())
	{
		Area->SetStaticMesh(CylinderAsset.Object);
	}

	Area->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Area->SetNotifyRigidBodyCollision(true);
	Area->BodyInstance.SetCollisionProfileName(TEXT("BlockAllDynamic"));

	Area->OnComponentHit.AddDynamic(this, &ABABumper::OnHit);
}

void ABABumper::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor) return;

	ABABall* Ball = Cast<ABABall>(OtherActor);
	if (Ball)
	{
		FVector PlayerVelocity = Ball->BallComponent->GetPhysicsLinearVelocity();
		FVector Normal = Hit.ImpactNormal;
		Normal.Z = 0;
		FVector ReflectedVector = PlayerVelocity.MirrorByVector(Normal.GetSafeNormal());

		Ball->BallComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
		Ball->BallComponent->AddImpulse(ReflectedVector.GetSafeNormal() * 1000, NAME_None, true);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "BABall.h"

// Sets default values
ABABall::ABABall()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallComponent"));
	RootComponent = BallComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereAsset.Succeeded())
	{
		BallComponent->SetStaticMesh(SphereAsset.Object);
	}

	//remove all physics system
	BallComponent->SetSimulatePhysics(false);
	BallComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ABABall::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(this))
	{
		UE_LOG(LogTemp, Error, TEXT("t"));
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
			}
		}
	}
}

// Called every frame
void ABABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
	//Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABABall::Move);
}

void ABABall::Jump()
{
	if (!CheckValidity()) return;

	BallComponent->AddForce(FVector(0, 0, JumpForce));
}

void ABABall::Move()
{
	if (!CheckValidity()) return;

	BallComponent->AddForce(FVector(0, 0, 0));
}

bool ABABall::CheckValidity()
{
	if (!BallComponent || !BallComponent->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Error, TEXT("[ABABall::CheckValidity] BallComponent not valid"));
		return false;
	}

	return true;
}


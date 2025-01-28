// Fill out your copyright notice in the Description page of Project Settings.


#include "BABall.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

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

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add the input mapping context
			if (PlayerMappingContext)
			{
				Subsystem->AddMappingContext(PlayerMappingContext, 0);
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

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IAMove)
		{
			EnhancedInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &ABABall::Move);
		}
		if (IAJump)
		{
			EnhancedInputComponent->BindAction(IAJump, ETriggerEvent::Started, this, &ABABall::Jump);
		}
	}
}

void ABABall::Jump(const FInputActionValue& Value)
{
	if (!CheckValidity()) return;

	UE_LOG(LogTemp, Error, TEXT("jump"));

	BallComponent->AddForce(FVector(0, 0, JumpForce));
}

void ABABall::Move(const FInputActionValue& Value)
{
	if (!CheckValidity()) return;
	UE_LOG(LogTemp, Error, TEXT("move"));

	FVector2D MoveDir = Value.Get<FVector2D>();
	BallComponent->AddForce(FVector(MoveDir.X, MoveDir.Y, 0) * Speed);
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


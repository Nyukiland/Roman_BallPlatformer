// Fill out your copyright notice in the Description page of Project Settings.


#include "BABall.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "BAMovablePlatform.h"

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

	BallComponent->SetSimulatePhysics(true);
	BallComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BallComponent->BodyInstance.bLockRotation = true;

	//create pivot and camera
	Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	Pivot->SetupAttachment(BallComponent);

	UCameraComponent* Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	Cam->SetupAttachment(Pivot);
	Cam->SetRelativeLocation(FVector(-300.0f, 0.0f, 100.0f));
	Cam->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
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
	GetGround();
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
		if (IALook)
		{
			EnhancedInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &ABABall::LookAround);
		}
		if (IADash)
		{
			EnhancedInputComponent->BindAction(IADash, ETriggerEvent::Started, this, &ABABall::DashPressed);
			EnhancedInputComponent->BindAction(IADash, ETriggerEvent::Canceled, this, &ABABall::DashReleased);
			EnhancedInputComponent->BindAction(IADash, ETriggerEvent::Completed, this, &ABABall::DashReleased);
		}
	}
}

void ABABall::Jump(const FInputActionValue& Value)
{
	if (!CheckValidity()) return;

	if (JumpCount < MaxJumpCount)
	{
		BallComponent->SetPhysicsLinearVelocity(FVector(BallComponent->GetPhysicsLinearVelocity().X, BallComponent->GetPhysicsLinearVelocity().Y, 0));
		BallComponent->AddImpulse(FVector(0, 0, JumpForce), NAME_None, true);
		++JumpCount;
	}
}

void ABABall::DashPressed(const FInputActionValue& Value)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
	CanMove = false;
}

void ABABall::DashReleased(const FInputActionValue& Value)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
	CanMove = true;

	BallComponent->SetPhysicsLinearVelocity(FVector::Zero());
	BallComponent->AddImpulse(Pivot->GetForwardVector() * DashStrength, NAME_None, true);
}

void ABABall::Move(const FInputActionValue& Value)
{
	if (!CheckValidity() || !CanMove) return;

	FVector2D MoveDir = Value.Get<FVector2D>();

	FVector Movement = Pivot->GetForwardVector() * MoveDir.X;
	Movement += Pivot->GetRightVector() * MoveDir.Y;
	if (Grounded) Movement *= Speed;
	else Movement *= Speed / 2;

	BallComponent->AddForce(Movement, NAME_None, true);
}

void ABABall::LookAround(const FInputActionValue& Value)
{
	if (!CheckValidity()) return;

	float Rot = Value.Get<float>();

	FRotator NewRotation = Pivot->GetRelativeRotation();
	NewRotation.Yaw += Rot * CamRotSpeed;

	Pivot->SetRelativeRotation(NewRotation);
}

void ABABall::GetGround()
{
	if (!CheckValidity()) return;

	FVector Start = BallComponent->GetComponentLocation();
	FVector End = Start + FVector(0, 0, -GroundCheckSize);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit;

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams))
	{
		Grounded = true;

		ABAMovablePlatform* HitPlatform = Cast<ABAMovablePlatform>(Hit.GetActor());
		if (HitPlatform) SetActorLocation(GetActorLocation() + HitPlatform->MovementValue * 0.05f);

		if (BallComponent->GetPhysicsLinearVelocity().Z <0) JumpCount = 0;
	}
	else
	{
		Grounded = false;
	}
}

bool ABABall::CheckValidity()
{
	if (!BallComponent || !BallComponent->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Error, TEXT("[ABABall::CheckValidity] BallComponent not valid"));
		return false;
	}

	if (!Pivot)
	{
		UE_LOG(LogTemp, Error, TEXT("[ABABall::CheckValidity] Pivot not valid"));
		return false;
	}

	return true;
}


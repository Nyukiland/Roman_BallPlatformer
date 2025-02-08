// Fill out your copyright notice in the Description page of Project Settings.


#include "BABall.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"

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
	}
}

void ABABall::Jump(const FInputActionValue& Value)
{
	if (!CheckValidity()) return;

	if (Grounded) BallComponent->AddImpulse(FVector(0, 0, JumpForce), NAME_None, true);
}

void ABABall::Move(const FInputActionValue& Value)
{
	if (!CheckValidity()) return;

	FVector2D MoveDir = Value.Get<FVector2D>();

	FVector Movement = Pivot->GetForwardVector() * MoveDir.X;
	Movement += Pivot->GetRightVector() * MoveDir.Y;
	Movement *= Speed;

	BallComponent->AddForce(Movement, NAME_None, false);
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

		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->GetRootComponent()->Mobility == EComponentMobility::Movable)
		{
			IsAttached = true;
			//AttachToComponent(HitActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		}
		else
		{
			//DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			IsAttached = false;
		}
	}
	else
	{
		Grounded = false;
		//if (!IsAttached) DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		IsAttached = false;
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


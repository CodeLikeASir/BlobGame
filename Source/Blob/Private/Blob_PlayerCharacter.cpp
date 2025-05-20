// Fill out your copyright notice in the Description page of Project Settings.

#include "Blob/Public/Blob_PlayerCharacter.h"
#include "Blob_PlayerShadow.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABlob_PlayerCharacter::ABlob_PlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetEnableGravity(true);
	CapsuleComponent->SetAngularDamping(10.0f);
	CapsuleComponent->SetLinearDamping(0.1f);

	// Lock rotation to prevent the capsule from falling over
	CapsuleComponent->SetConstraintMode(EDOFMode::SixDOF);
	CapsuleComponent->GetBodyInstance()->bLockXRotation = true;
	CapsuleComponent->GetBodyInstance()->bLockYRotation = true;
	RootComponent = CapsuleComponent;

	BlobMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlobMesh"));
	BlobMesh->SetupAttachment(RootComponent);

	EyeLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeLeft"));
	EyeLeft->SetupAttachment(BlobMesh);
	EyeRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeRight"));
	EyeRight->SetupAttachment(BlobMesh);

	bIsGrounded = false;
	InputVec = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void ABlob_PlayerCharacter::BeginPlay()
{
	if (PlayerShadowClass == nullptr)
		return;

	PlayerShadow = GetWorld()->SpawnActor<ABlob_PlayerShadow>(PlayerShadowClass);
	PlayerShadow->PlayerCharacter = this;

	BaseScale = BlobMesh->GetRelativeScale3D();
	BasePitch = EyeLeft->GetRelativeRotation().Pitch;

	BaseMass = CapsuleComponent->GetMass();

	Super::BeginPlay();
}

// Called every frame
void ABlob_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateChargeProgress(DeltaTime);
	UpdateGrounded();
	ApplyMovementForce(DeltaTime);
	LimitVelocity();
}

void ABlob_PlayerCharacter::UpdateChargeProgress(float DeltaTime)
{
	if (bIsChargingJump && ChargeProgress < 1.0f)
	{
		float DeltaProgress = DeltaTime / MaxChargeTime;
		ChargeProgress = FMath::Clamp(ChargeProgress + DeltaProgress, 0.0f, 1.0f);

		float XYScale = FMath::Lerp(BaseScale.X, MaxXYScale, ChargeProgress);
		float ZScale = FMath::Lerp(BaseScale.Z, MinZScale, ChargeProgress);
		BlobMesh->SetRelativeScale3D(FVector(XYScale, XYScale, ZScale));

		float Pitch = FMath::Lerp(BasePitch, MaxPitch, ChargeProgress);
		EyeLeft->SetRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));
		EyeRight->SetRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));
	}
}

void ABlob_PlayerCharacter::UpdateGrounded()
{
	FVector StartLocation = BlobMesh->GetComponentLocation();
	FVector EndLocation = StartLocation - FVector(0.f, 0.f, GroundedDistance);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	if (FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation,
											 ECC_Visibility, TraceParams))
	{
		bIsGrounded = HitResult.Distance < GroundedDistance;
	}
	else
	{
		bIsGrounded = false;
	}
}

void ABlob_PlayerCharacter::ChargeJump()
{
	bIsChargingJump = true;
}

void ABlob_PlayerCharacter::ReleaseJump()
{
	if (bIsGrounded)
	{
		float JumpZVelocity = MaxJumpVelocity * ChargeProgress;
		CapsuleComponent->AddImpulse(FVector(0.0f, 0.0f, JumpZVelocity * JumpForceScalar), NAME_None, true);
	}

	bIsChargingJump = false;
	ChargeProgress = 0.0f;

	SquishScale = BlobMesh->GetRelativeScale3D();
	EyeLeft->SetRelativeRotation(FRotator(BasePitch, 0.0f, 0.0f));
	EyeRight->SetRelativeRotation(FRotator(BasePitch, 0.0f, 0.0f));

	ReleaseJumpBP();
}

void ABlob_PlayerCharacter::AddMovementInput(FVector NewInputVec)
{
	this->InputVec = NewInputVec;
}

void ABlob_PlayerCharacter::StartDownforce()
{
	CapsuleComponent->SetMassScale(NAME_None, DownforceMassMultiplier);
	CapsuleComponent->AddForce(FVector::DownVector * DownforceForce, NAME_None, true);
	StartDownforceBP();
}

void ABlob_PlayerCharacter::StopDownforce()
{
	CapsuleComponent->SetMassScale(NAME_None, 1.0f);
	StopDownforceBP();
}

void ABlob_PlayerCharacter::CancelMove()
{
	InputVec = FVector::ZeroVector;
}

void ABlob_PlayerCharacter::ApplyMovementForce(float DeltaTime)
{
	// Calculate the movement vector based on input
	FVector MoveInput = FVector(InputVec.X, InputVec.Y, 0.f);

	// Apply force based on whether the character is grounded
	float ForceMultiplier = bIsGrounded ? 1.0f : AirControl;
	if (MoveInput.Length() > 0.1f)
	{
		// Apply the movement force
		FVector Acceleration = MoveInput * MovementForce * ForceMultiplier * DeltaTime;
		CapsuleComponent->SetPhysicsLinearVelocity(Acceleration, true);
	}

	// Apply extra friction when grounded and not providing input
	if (bIsGrounded && MoveInput.Length() < .2f)
	{
		FVector Velocity = CapsuleComponent->GetPhysicsLinearVelocity();
		Velocity.X *= TimeToStop - DeltaTime;
		Velocity.Y *= TimeToStop - DeltaTime;
		CapsuleComponent->SetPhysicsLinearVelocity(Velocity);
	}

	LimitVelocity();
	RotateMesh(DeltaTime);
}

void ABlob_PlayerCharacter::RotateMesh(float DeltaTime)
{
	// Rotate actor towards move direction
	FVector MoveVec = CapsuleComponent->GetComponentVelocity();
	MoveVec.Z = 0.0f;
	if (MoveVec.Length() > 50.0f)
	{
		FRotator Rotation = FRotationMatrix::MakeFromX(MoveVec).Rotator();
		Rotation.Pitch = 0.0f;
		Rotation.Roll = 0.0f;
		BlobMesh->SetWorldRotation(
			FMath::RInterpTo(BlobMesh->GetComponentRotation(),
			                 Rotation, DeltaTime, 25.0f));
	}
}

void ABlob_PlayerCharacter::LimitVelocity()
{
	// Get current velocity
	FVector CurrentVelocity = CapsuleComponent->GetPhysicsLinearVelocity();

	// Extract horizontal velocity (XY plane)
	FVector HorizontalVelocity = FVector(CurrentVelocity.X, CurrentVelocity.Y, 0.0f);

	// Check if the horizontal speed exceeds the maximum
	if (HorizontalVelocity.Size() > MaxSpeed)
	{
		// Calculate the limited horizontal velocity
		HorizontalVelocity = HorizontalVelocity.GetSafeNormal() * MaxSpeed;

		// Update the velocity with the limited horizontal component
		CapsuleComponent->SetPhysicsLinearVelocity(FVector(HorizontalVelocity.X, HorizontalVelocity.Y,
		                                                   CurrentVelocity.Z));
	}
}

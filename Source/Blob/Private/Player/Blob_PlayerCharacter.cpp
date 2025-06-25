// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Blob_PlayerCharacter.h"
#include "Player/Blob_PlayerShadow.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);

	DynamicCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DynamicCamera"));
	DynamicCamera->SetupAttachment(CameraArm);
	
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
	
	if (!bUnlockedVelocity)
	{
		ApplyMovementForce(DeltaTime);
		LimitVelocity();
		ApplyGroundVelocity();
	}
	
	RotateMesh(DeltaTime);
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

		FVector EyeScale = FVector()
		EyeLeft->SetRelativeScale3D(FVector());
	}
}

void ABlob_PlayerCharacter::RotateCamera(FVector2D Input)
{
	AddControllerYawInput(Input.X);
	AddControllerPitchInput(-Input.Y);
}

FVector ABlob_PlayerCharacter::ToLocalSpace(FVector WorldSpace)
{
	return DynamicCamera->GetComponentRotation().RotateVector(FVector(WorldSpace.X, WorldSpace.Y, 0.0f));
}

void ABlob_PlayerCharacter::UnlockVelocity(const float UnlockTime)
{
	bUnlockedVelocity = true;
	GetWorldTimerManager().SetTimer(TimerHandle_LockVelocity, this, &ABlob_PlayerCharacter::LockVelocity,
		UnlockTime, false);
}

void ABlob_PlayerCharacter::UpdateGrounded()
{
	FVector StartLocation = BlobMesh->GetComponentLocation();
	FVector EndLocation = StartLocation - FVector(0.f, 0.f, GroundedDistance);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	float Radius = GetCapsuleComponent()->GetScaledCapsuleRadius() * 0.75f;
	TArray<AActor*> ActorsToIgnore = {this};
	if (FHitResult HitResult; UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(), StartLocation, EndLocation, Radius,
		TraceTypeQuery1, false, ActorsToIgnore,
		EDrawDebugTrace::None,HitResult, true))
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
	FVector MoveInput = FVector(InputVec.X, InputVec.Y, 0.f);
	
	if (CheckWallStuck(MoveInput))
		return;
	
	// Apply force based on whether the character is grounded
	float ForceMultiplier = bIsGrounded ? 1.0f : AirControl;
	if (MoveInput.Length() > 0.1f)
	{
		// Apply the movement force
		FVector Acceleration = MoveInput * MovementForce * ForceMultiplier * DeltaTime;
		CapsuleComponent->SetPhysicsLinearVelocity(Acceleration, true);
	}

	// Apply extra friction when grounded and not providing input
	if (MoveInput.Length() < .2f)
	{
		float FrictionMultiplier = bIsGrounded ? GroundFriction : AirFriction;
		FVector Friction = -CapsuleComponent->GetPhysicsLinearVelocity() * FrictionMultiplier * DeltaTime;
		Friction.Z = 0.0f;
		CapsuleComponent->SetPhysicsLinearVelocity(Friction, true);
	}
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
	}

	// Update the Z component of the velocity
	CapsuleComponent->SetPhysicsLinearVelocity(FVector(HorizontalVelocity.X, HorizontalVelocity.Y,
	                                                   FMath::Clamp(-MaxZVelocity, CurrentVelocity.Z, MaxZVelocity)));
}

void ABlob_PlayerCharacter::ApplyGroundVelocity() const
{
	FVector Velocity = CapsuleComponent->GetPhysicsLinearVelocity() + GroundVelocity;
	CapsuleComponent->SetPhysicsLinearVelocity(Velocity);
}

bool ABlob_PlayerCharacter::CheckWallStuck(FVector MoveInput)
{
	if (bIsGrounded || MoveInput.Size() < 0.01f)
		return false;

	// Ray trace from the center of the blob to the current position
	FVector StartLocation = BlobMesh->GetComponentLocation() +
		FVector::UpVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector EndLocation = StartLocation + MoveInput.GetSafeNormal() * 60.0f;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	float Radius = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.75f;
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore = {this};
	return UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLocation, EndLocation, Radius,
	TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None,
	HitResult, true);
}

void ABlob_PlayerCharacter::LockVelocity()
{
	bUnlockedVelocity = false;
}

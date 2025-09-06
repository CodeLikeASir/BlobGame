#include "Player/Blob_PlayerCharacter.h"
#include "Systems/Blob_Settings.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
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

ABlob_PlayerCharacter::~ABlob_PlayerCharacter()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_LockVelocity);
	}
}

void ABlob_PlayerCharacter::UnblockInput()
{
	bBlockInput = false;
}

void ABlob_PlayerCharacter::OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                         class AController* InstigatedBy, AActor* DamageCauser)
{
	bBlockInput = true;
	GetWorldTimerManager().SetTimer(TH_BlockInput, this, &ABlob_PlayerCharacter::UnblockInput, Damage, false);
}

// Called when the game starts or when spawned
void ABlob_PlayerCharacter::BeginPlay()
{
	BaseScale = BlobMesh->GetRelativeScale3D();
	BasePitch = EyeLeft->GetRelativeRotation().Pitch;

	BaseMass = CapsuleComponent->GetMass();
	LoadSettings();

	OnTakeAnyDamage.AddDynamic(this, &ABlob_PlayerCharacter::OnTakeDamage);
	
	Super::BeginPlay();
}

// Called every frame
void ABlob_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateChargeProgress(DeltaTime);
	UpdateGrounded();

	if (!bIsStunned)
	{
		ApplyMovementForce(DeltaTime);
		LimitVelocity();
		ApplyGroundVelocity();
		CheckWallStuck(FVector(InputVec.X, InputVec.Y, 0.f));
		RotateMesh(DeltaTime);
	}
}

void ABlob_PlayerCharacter::UpdateChargeProgress(float DeltaTime)
{
	if (bIsChargingJump && ChargeProgress < 1.0f)
	{
		float DeltaProgress = DeltaTime / MaxChargeTime;
		ChargeProgress = FMath::Clamp(ChargeProgress + DeltaProgress, 0.0f, 1.0f);

		constexpr float Min_Scale = 0.01f; // Prevent zero/negative scale
		float XYScale = FMath::Max(FMath::Lerp(BaseScale.X, MaxXYScale, ChargeProgress), Min_Scale);
		float ZScale = FMath::Max(FMath::Lerp(BaseScale.Z, MinZScale, ChargeProgress), Min_Scale);
		BlobMesh->SetRelativeScale3D(FVector(XYScale, XYScale, ZScale));

		float Pitch = FMath::Lerp(BasePitch, MaxPitch, ChargeProgress);
		EyeLeft->SetRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));
		EyeRight->SetRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));
	}
}

void ABlob_PlayerCharacter::RotateCamera(FVector2D Input)
{
	AddControllerYawInput(Input.X * Settings->CameraSensitivity);
	AddControllerPitchInput(-Input.Y * Settings->CameraSensitivity);
}

FVector ABlob_PlayerCharacter::ToLocalSpace(FVector WorldSpace)
{
	return DynamicCamera->GetComponentRotation().RotateVector(FVector(WorldSpace.X, WorldSpace.Y, 0.0f));
}

void ABlob_PlayerCharacter::BeginHitReaction(const float UnlockTime)
{
	bIsStunned = true;
	GetWorldTimerManager().SetTimer(TimerHandle_LockVelocity, this, &ABlob_PlayerCharacter::EndHitReaction,
	                                UnlockTime, false);

	BeginHitReactionBP(UnlockTime);
}

void ABlob_PlayerCharacter::UpdateGrounded()
{
	FScopeLock Lock(&GroundedLock);

	FVector StartLocation = BlobMesh->GetComponentLocation();
	FVector EndLocation = StartLocation - FVector(0.f, 0.f, GroundedDistance);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	float Radius = GetCapsuleComponent()->GetScaledCapsuleRadius() * 0.75f;
	TArray<AActor*> ActorsToIgnore = {this};
	if (FHitResult HitResult; UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(), StartLocation, EndLocation, Radius,
		TraceTypeQuery1, false, ActorsToIgnore,
		EDrawDebugTrace::None, HitResult, true))
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

void ABlob_PlayerCharacter::AddMovementInput(const FVector& NewInputVec)
{
	this->InputVec = NewInputVec;
}

void ABlob_PlayerCharacter::StartDownforce()
{
	CapsuleComponent->SetMassScale(NAME_None, DownforceMassMultiplier);
	CapsuleComponent->AddForce(FVector::DownVector * DownforceForce, NAME_None, true);
	StartDownforceBP();
}

void ABlob_PlayerCharacter::AddForceCustom(FVector Force, float StunLength)
{
	BeginHitReaction(StunLength);
	CapsuleComponent->AddForce(Force, NAME_None, true);
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

void ABlob_PlayerCharacter::LoadSettings()
{
	if (Settings)
	{
		Settings->RemoveFromRoot(); // Remove old settings from root
	}

	UBlob_Settings* LoadedSettings = Cast<UBlob_Settings>(UGameplayStatics::LoadGameFromSlot("Settings", 0));
	if (!LoadedSettings)
	{
		LoadedSettings = Cast<UBlob_Settings>(UGameplayStatics::CreateSaveGameObject(UBlob_Settings::StaticClass()));
		UGameplayStatics::SaveGameToSlot(LoadedSettings, "Settings", 0);
	}
	Settings = LoadedSettings;
	Settings->AddToRoot(); // Prevent garbage collection
}

void ABlob_PlayerCharacter::ApplyMovementForce(float DeltaTime)
{
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
	if (MoveInput.Length() < .2f)
	{
		float FrictionMultiplier = bIsGrounded ? GroundFriction : AirFriction;
		FVector Friction = -CapsuleComponent->GetPhysicsLinearVelocity() * FrictionMultiplier * DeltaTime;
		Friction.Z = 0.0f;
		CapsuleComponent->SetPhysicsLinearVelocity(Friction, true);
	}
}

void ABlob_PlayerCharacter::RotateMesh(float DeltaTime) const
{
	if (!CapsuleComponent || !BlobMesh)
		return;

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

void ABlob_PlayerCharacter::LimitVelocity() const
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
	if (bIsGrounded)
		return false;

	
	// In CheckWallStuck function, replace the LineTraceSingleByChannel with:
	FHitResult Hit;
	FVector StartLocation = GetActorLocation() + FVector::DownVector * CapsuleComponent->GetScaledCapsuleHalfHeight();
	FVector ForwardTrace = StartLocation + MoveInput * WallStuckCheckRange;

	// Set up trace parameters
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	// Use sphere trace instead of line trace
	const float SphereRadius = WallStuckCheckRadius; // Adjust radius as needed
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartLocation,
		ForwardTrace,
		SphereRadius,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false, // bTraceComplex
		TArray<AActor*>{this}, // ActorsToIgnore
		EDrawDebugTrace::None, // Debug visualization
		Hit,
		true // bIgnoreSelf
	);

	if (bHit)
	{
		FVector WallNormal = Hit.Normal;

		// Only handle vertical walls (not floors/ceilings)
		if (FMath::Abs(WallNormal.Z) < 0.1f)
		{
			// Project velocity along the wall surface
			FVector Velocity = CapsuleComponent->GetPhysicsLinearVelocity();
			FVector ProjectedVelocity = FVector::VectorPlaneProject(Velocity, WallNormal);

			// Keep vertical component, remove horizontal component toward wall
			ProjectedVelocity.Z = -196.0f;

			CapsuleComponent->SetPhysicsLinearVelocity(ProjectedVelocity);
		}
	}

	return true;
}

void ABlob_PlayerCharacter::EndHitReaction()
{
	bIsStunned = false;
	EndHitReactionBP();
}

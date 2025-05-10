// Fill out your copyright notice in the Description page of Project Settings.

#include "Blob/Public/Blob_PlayerCharacter.h"

#include "Blob_PlayerShadow.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Sets default values
ABlob_PlayerCharacter::ABlob_PlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlaceholderMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaceholderMesh"));
	PlaceholderMeshComponent->SetupAttachment(RootComponent);

	EyeLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeLeft"));
	EyeLeft->SetupAttachment(RootComponent);
	EyeRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeRight"));
	EyeRight->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABlob_PlayerCharacter::BeginPlay()
{
	if (PlayerShadowClass == nullptr)
		return;
	
	PlayerShadow = GetWorld()->SpawnActor<ABlob_PlayerShadow>(PlayerShadowClass);
	PlayerShadow->PlayerCharacter = this;

	BaseScale = PlaceholderMeshComponent->GetRelativeScale3D();
	BasePitch = EyeLeft->GetRelativeRotation().Pitch;
	
	Super::BeginPlay();
}

// Called every frame
void ABlob_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsChargingJump && ChargeProgress < 1.0f)
	{
		float DeltaProgress = DeltaTime / MaxChargeTime;
		ChargeProgress = FMath::Clamp(ChargeProgress + DeltaProgress, 0.0f, 1.0f);

		float XYScale = FMath::Lerp(BaseScale.X, MaxXYScale, ChargeProgress);
		float ZScale = FMath::Lerp(BaseScale.Z, MinZScale, ChargeProgress);
		PlaceholderMeshComponent->SetRelativeScale3D(FVector(XYScale, XYScale, ZScale));

		float Pitch = FMath::Lerp(BasePitch, MaxPitch, ChargeProgress);
		EyeLeft->SetRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));
		EyeRight->SetRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));
	}
}

void ABlob_PlayerCharacter::ChargeJump()
{
	bIsChargingJump = true;
}

void ABlob_PlayerCharacter::ReleaseJump()
{
	GetCharacterMovement()->JumpZVelocity = MaxJumpVelocity * ChargeProgress;
	Jump();

	bIsChargingJump = false;
	ChargeProgress = 0.0f;

	SquishScale = PlaceholderMeshComponent->GetRelativeScale3D();
	EyeLeft->SetRelativeRotation(FRotator(BasePitch, 0.0f, 0.0f));
	EyeRight->SetRelativeRotation(FRotator(BasePitch, 0.0f, 0.0f));
	
	ReleaseJumpBP();
}

void ABlob_PlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
    
	// Handle bounce when landing
	HandleBounce(Hit);
}

void ABlob_PlayerCharacter::HandleBounce(const FHitResult& Hit)
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp)
	{
		// Get the velocity before landing
		float FallVelocity = FMath::Abs(MovementComp->Velocity.Z);
        
		// Only bounce if falling fast enough
		if (FallVelocity > BounceThreshold)
		{
			MovementComp->Velocity = MovementComp->Velocity * FVector(0.5f, 0.5f, 1.0f);
			// Calculate bounce velocity based on fall velocity and bounce factor
			float BounceVelocity = FallVelocity * BounceFactor;
            
			// Launch character back up
			LaunchCharacter(FVector(0.0f, 0.0f, BounceVelocity), false, true);
			return;
		}
	}
}
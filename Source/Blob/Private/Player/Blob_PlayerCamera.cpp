// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Blob_PlayerCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/Blob_PlayerCharacter.h"

// Sets default values
ABlob_PlayerCamera::ABlob_PlayerCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArmRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
	RootComponent = ArmRootComponent;

	CameraArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArmComponent"));
	CameraArmComponent->TargetArmLength = 1000.f;
	CameraArmComponent->bUsePawnControlRotation = true;
	CameraArmComponent->SetupAttachment(ArmRootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
}

// Called when the game starts or when spawned
void ABlob_PlayerCamera::BeginPlay()
{
	Super::BeginPlay();
	CameraComponent->AttachToComponent(CameraArmComponent, FAttachmentTransformRules::SnapToTargetIncludingScale,
		CameraArmComponent->SocketName);
}

// Called every frame
void ABlob_PlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter == nullptr)
		PlayerCharacter = Cast<ABlob_PlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	float TargetZ = PlayerCharacter ? PlayerCharacter->GetActorLocation().Z + CameraTargetOffset : 0.0f;
	float DeltaZ = TargetZ - GetActorLocation().Z;
	DeltaZ = FMath::Clamp(DeltaZ, -5.0f, 5.0f);
	float SmoothedZ = FMath::FInterpTo(GetActorLocation().Z, TargetZ, DeltaTime, 2.0f);
	if (FMath::Abs(DeltaZ) > 1.0f)
	{
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, SmoothedZ));
	}
}

void ABlob_PlayerCamera::RotateCamera(FVector2D Value)
{
	FRotator Rotation = FRotator::MakeFromEuler(FVector(0.0f, Value.Y, Value.X));
	Rotation.Pitch *= GetWorld()->GetDeltaSeconds() * Sensitivity;
	Rotation.Yaw *= GetWorld()->GetDeltaSeconds() * Sensitivity;
	CameraArmComponent->AddRelativeRotation(Rotation);
}

void ABlob_PlayerCamera::MoveCameraUpDown(float Value)
{
	AddActorLocalOffset(FVector::UpVector * Value);
}

void ABlob_PlayerCamera::ResetCamera()
{
	CameraArmComponent->SetRelativeRotation(FRotator::ZeroRotator);
	SetActorLocation(FVector(0.0f, 0.0f, PlayerCharacter->GetActorLocation().Z + CameraTargetOffset));
}

FRotator ABlob_PlayerCamera::GetCurrentRotation()
{
	return CameraArmComponent->GetRelativeRotation();
}

FVector ABlob_PlayerCamera::GetCameraLocation()
{
	return CameraComponent->GetComponentLocation();
}

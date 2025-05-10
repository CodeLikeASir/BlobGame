// Fill out your copyright notice in the Description page of Project Settings.
#include "Blob/Public/Blob_PlayerController.h"

#include "Blob_PlayerCamera.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "Blob/Public/Blob_PlayerCharacter.h"

void ABlob_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ABlob_PlayerCharacter>(GetPawn());
	CustomPlayerCamera = GetWorld()->SpawnActor<ABlob_PlayerCamera>(PlayerCameraClass);

	if (!CustomPlayerCamera)
		return;
	
	CustomPlayerCamera->PlayerCharacter = PlayerCharacter;
	CustomPlayerCamera->SetActorLocation(FVector(0.0f, 0.0f, 50.0f));
	
	if (CustomPlayerCamera && PlayerCharacter)
	{
		// Activate the CustomPlayerCamera to ensure it is the current view target
		SetViewTarget(CustomPlayerCamera);
	}
}

void ABlob_PlayerController::Respawn()
{
	RespawnBP();
}

void ABlob_PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABlob_PlayerController::OnMove);

		// Jump
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ABlob_PlayerController::ChargeJump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ABlob_PlayerController::ReleaseJump);

		// Camera Controls
		EnhancedInputComponent->BindAction(IA_RotateCamera, ETriggerEvent::Triggered, this, &ABlob_PlayerController::RotateCamera);
		EnhancedInputComponent->BindAction(IA_MoveCameraUpDown, ETriggerEvent::Triggered, this, &ABlob_PlayerController::MoveCameraUpDown);

		// Respawn
		EnhancedInputComponent->BindAction(IA_Respawn, ETriggerEvent::Started, this, &ABlob_PlayerController::Respawn);
	}
}

void ABlob_PlayerController::OnMove(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	PlayerCharacter->AddMovementInput(CustomPlayerCamera->GetCurrentRotation()
		.RotateVector(FVector(Value.X, Value.Y, 0.0f)));
}

void ABlob_PlayerController::ChargeJump(const FInputActionValue& InputActionValue)
{
	PlayerCharacter->ChargeJump();
}

void ABlob_PlayerController::ReleaseJump(const FInputActionValue& InputActionValue)
{
	PlayerCharacter->ReleaseJump();
}

void ABlob_PlayerController::RotateCamera(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	CustomPlayerCamera->RotateCamera(Value * CameraRotationSpeed);
}

void ABlob_PlayerController::MoveCameraUpDown(const FInputActionValue& InputActionValue)
{
	const float Value = InputActionValue.Get<float>();
	CustomPlayerCamera->MoveCameraUpDown(Value * CameraMoveSpeed);
}

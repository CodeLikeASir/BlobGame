#include "Player/Blob_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "Player/Blob_PlayerCharacter.h"

void ABlob_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ABlob_PlayerCharacter>(GetPawn());
	if(PlayerCharacter == nullptr)
		return;
	BaseScale = PlayerCharacter->GetActorScale3D();
}

void ABlob_PlayerController::Respawn()
{
	RespawnBP();
}

void ABlob_PlayerController::OnCheckpointReached(int Index)
{
	OnCheckpointReachedBP(Index);
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
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Canceled, this, &ABlob_PlayerController::CancelMove);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &ABlob_PlayerController::CancelMove);

		// Jump
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ABlob_PlayerController::ChargeJump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ABlob_PlayerController::ReleaseJump);

		// Downoforce
		EnhancedInputComponent->BindAction(IA_Downforce, ETriggerEvent::Started, this, &ABlob_PlayerController::StartDownforce);
		EnhancedInputComponent->BindAction(IA_Downforce, ETriggerEvent::Completed, this, &ABlob_PlayerController::StopDownforce);

		// Camera Controls
		EnhancedInputComponent->BindAction(IA_RotateCamera, ETriggerEvent::Triggered, this, &ABlob_PlayerController::RotateCamera);

		// Respawn
		EnhancedInputComponent->BindAction(IA_Respawn, ETriggerEvent::Started, this, &ABlob_PlayerController::Respawn);

		// Savestate
		EnhancedInputComponent->BindAction(IA_SetSaveState, ETriggerEvent::Started, this, &ABlob_PlayerController::SetSaveState);
		EnhancedInputComponent->BindAction(IA_LoadSaveState, ETriggerEvent::Started, this, &ABlob_PlayerController::LoadSaveState);

		// Pause Menu
		EnhancedInputComponent->BindAction(IA_PauseMenu, ETriggerEvent::Started, this, &ABlob_PlayerController::OpenPauseMenu);
	}
}

void ABlob_PlayerController::CancelMove(const FInputActionValue& InputActionValue)
{
	PlayerCharacter->CancelMove();
}

void ABlob_PlayerController::OnMove(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	FVector MoveInput = PlayerCharacter->ToLocalSpace(FVector(Value.X, Value.Y, 0.0f));
	PlayerCharacter->AddMovementInput(MoveInput);
}

void ABlob_PlayerController::ChargeJump(const FInputActionValue& InputActionValue)
{
	PlayerCharacter->ChargeJump();
}

void ABlob_PlayerController::ReleaseJump(const FInputActionValue& InputActionValue)
{
	PlayerCharacter->ReleaseJump();
}

void ABlob_PlayerController::StartDownforce(const FInputActionValue& InputActionValue)
{
	PlayerCharacter->StartDownforce();
}

void ABlob_PlayerController::StopDownforce(const FInputActionValue& InputActionValue)
{
	PlayerCharacter->StopDownforce();
}

void ABlob_PlayerController::SetSaveState(const FInputActionValue& InputActionValue)
{
	FTransform Transform = PlayerCharacter->GetActorTransform();
	Transform.SetScale3D(BaseScale);
	SaveStateTransform = Transform;

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Saved"));
}

void ABlob_PlayerController::LoadSaveState(const FInputActionValue& InputActionValue)
{
	PlayerCharacter->SetActorTransform(SaveStateTransform);
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Loaded"));
}

void ABlob_PlayerController::RotateCamera(const FInputActionValue& InputActionValue)
{
	if (GetWorld()->TimeSeconds < 1.0f)
		return;
	
	const FVector2D Value = InputActionValue.Get<FVector2D>() * CameraRotationSpeed;
	PlayerCharacter->RotateCamera(Value);
}
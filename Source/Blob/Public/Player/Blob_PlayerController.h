#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Blob_PlayerController.generated.h"

class UInputMappingContext;
class ABlob_PlayerCharacter;

/**
 * Player Controller handling input, camera controls, checkpoints, save/load, and UI hooks.
 */
UCLASS()
class BLOB_API ABlob_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// ---- APlayerController overrides ----
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// ---- Input handlers ----
	void OnMove(const FInputActionValue& InputActionValue);
	void ChargeJump(const FInputActionValue& InputActionValue);
	void ReleaseJump(const FInputActionValue& InputActionValue);
	void StartDownforce(const FInputActionValue& InputActionValue);
	void StopDownforce(const FInputActionValue& InputActionValue);
	void RotateCamera(const FInputActionValue& InputActionValue);
	void CancelMove(const FInputActionValue& InputActionValue);

	// ---- Save/Load state ----
	void SetSaveState(const FInputActionValue& InputActionValue);
	void LoadSaveState(const FInputActionValue& InputActionValue);

	// ---- Checkpoints / Flow ----
	UFUNCTION(BlueprintCallable)
	void JumpToNextCheckpoint();

	UFUNCTION()
	void OnCheckpointReached(int Index);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCheckpointReachedBP(int Index);

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameEndBP();

	// ---- Respawn ----
	void Respawn();

	UFUNCTION(BlueprintImplementableEvent)
	void RespawnBP();

	// ---- UI / Notifications ----
	UFUNCTION(BlueprintImplementableEvent)
	void OpenPauseMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowResetNotification();

	UFUNCTION(BlueprintImplementableEvent)
	void ClearNotifications();

	// ---- References ----
	UPROPERTY(BlueprintReadOnly)
	ABlob_PlayerCharacter* PlayerCharacter;

	// ---- Input mapping ----
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// ---- Input actions ----
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Downforce;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_RotateCamera;

	UPROPERTY(EditAnywhere, Category = Input)
	const UInputAction* IA_MoveCameraUpDown;

	UPROPERTY(EditAnywhere, Category = Input)
	const UInputAction* IA_SwitchCamera;

	UPROPERTY(EditAnywhere, Category = Input)
	const UInputAction* IA_Respawn;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_PauseMenu;

	// ---- Tuning ----
	UPROPERTY(EditAnywhere, Category = Input)
	float CameraRotationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = Input)
	float CameraMoveSpeed = 5.0f;

	// ---- Runtime state ----
	UPROPERTY()
	FTransform SaveStateTransform;

	FVector BaseScale;
};

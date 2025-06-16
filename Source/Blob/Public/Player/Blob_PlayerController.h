// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Blob_PlayerController.generated.h"

class ABlob_PlayerCamera;
class UInputMappingContext;
class ABlob_PlayerCharacter;
/**
 * 
 */

UCLASS()
class BLOB_API ABlob_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	void StartDownforce(const FInputActionValue& InputActionValue);
	void StopDownforce(const FInputActionValue& InputActionValue);
	void SetSaveState(const FInputActionValue& InputActionValue);
	void LoadSaveState(const FInputActionValue& InputActionValue);
	virtual void SetupInputComponent() override;
	void CancelMove(const FInputActionValue& InputActionValue);

	void OnMove(const FInputActionValue& InputActionValue);
	void ChargeJump(const FInputActionValue& InputActionValue);
	void ReleaseJump(const FInputActionValue& InputActionValue);
	void RotateCamera(const FInputActionValue& InputActionValue);
	void MoveCameraUpDown(const FInputActionValue& InputActionValue);
	
	void Respawn();
	
	UFUNCTION(BlueprintImplementableEvent)
	void RespawnBP();

	UPROPERTY(BlueprintReadOnly)
	ABlob_PlayerCharacter* PlayerCharacter;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category= Input)
	UInputAction* IA_Move;
	
	UPROPERTY(EditAnywhere, Category= Input)
	UInputAction* IA_Jump;
	
	UPROPERTY(EditAnywhere, Category= Input)
	UInputAction* IA_Downforce;
	
	UPROPERTY(EditAnywhere, Category= Input)
	UInputAction* IA_RotateCamera;

	UPROPERTY(EditAnywhere, Category= Input)
	const UInputAction* IA_MoveCameraUpDown;

	UPROPERTY(EditAnywhere, Category= Input)
	const UInputAction* IA_SwitchCamera;
	
	UPROPERTY(EditAnywhere, Category= Input)
	const UInputAction* IA_Respawn;
	
	UPROPERTY(EditAnywhere, Category= Input)
	UInputAction* IA_SetSaveState;
	
	UPROPERTY(EditAnywhere, Category= Input)
	UInputAction* IA_LoadSaveState;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABlob_PlayerCamera> PlayerCameraClass;

	UPROPERTY(BlueprintReadOnly)
	ABlob_PlayerCamera* CustomPlayerCamera;

	UPROPERTY(EditAnywhere, Category= Input)
	float CameraRotationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category= Input)
	float CameraMoveSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category= "Camera")
	bool bUseStaticCamera;

	UPROPERTY()
	FTransform SaveStateTransform;
	FVector BaseScale;
};

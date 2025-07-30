

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Blob_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUINavigateSignature, const FInputActionValue&, InputActionValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUIAcceptSignature, const FInputActionValue&, InputActionValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUICancelSignature, const FInputActionValue&, InputActionValue);

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

	UFUNCTION(BlueprintImplementableEvent)
	void OpenPauseMenu();

	virtual void SetupInputComponent() override;
	void UI_Navigate(const FInputActionValue& InputActionValue);
	void UI_Accept(const FInputActionValue& InputActionValue);
	void UI_Cancel(const FInputActionValue& InputActionValue);
	void CancelMove(const FInputActionValue& InputActionValue);

	void OnMove(const FInputActionValue& InputActionValue);
	void ChargeJump(const FInputActionValue& InputActionValue);
	void ReleaseJump(const FInputActionValue& InputActionValue);
	void RotateCamera(const FInputActionValue& InputActionValue);

	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnUINavigateSignature OnUINavigate;

	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnUIAcceptSignature OnUIAccept;

	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnUICancelSignature OnUICancel;
	
	void Respawn();
	
	UFUNCTION(BlueprintImplementableEvent)
	void RespawnBP();

	UFUNCTION()
	void OnCheckpointReached(int Index);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCheckpointReachedBP(int Index);

	UPROPERTY(BlueprintReadOnly)
	ABlob_PlayerCharacter* PlayerCharacter;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* UIMappingContext;

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
	
	UPROPERTY(EditAnywhere, Category= Input)
	UInputAction* IA_PauseMenu;
	
	UPROPERTY(EditAnywhere, Category= Input)
	const UInputAction* IA_UI_Navigate;
	
	UPROPERTY(EditAnywhere, Category= Input)
	const UInputAction* IA_UI_Accept;
	
	UPROPERTY(EditAnywhere, Category= Input)
	const UInputAction* IA_UI_Cancel;

	UPROPERTY(EditAnywhere, Category= Input)
	float CameraRotationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category= Input)
	float CameraMoveSpeed = 5.0f;

	UPROPERTY()
	FTransform SaveStateTransform;
	FVector BaseScale;
};

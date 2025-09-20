#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blob_PlayerCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;

/**
 * Class for controlling the player incl. configuration of all gameplay features
 */
UCLASS()
class BLOB_API ABlob_PlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	ABlob_PlayerCharacter();
	virtual ~ABlob_PlayerCharacter() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }

	UPROPERTY(EditAnywhere)
	FVector GroundVelocity;

	UPROPERTY(EditAnywhere)
	float MinCameraHeight;

	// Movement & Forces
	void ApplyGroundVelocity() const;
	void AddMovementInput(const FVector& NewInputVec);
	UFUNCTION()
	void AddStunForce(const FVector& Force, float StunLength);
	void CancelMove();

	// Jump
	UFUNCTION()
	void ChargeJump();
	UFUNCTION()
	void ReleaseJump();
	void UpdateChargeProgress(float DeltaTime);

	// Downforce
	void StartDownforce();

	UFUNCTION(BlueprintCallable)
	void StopDownforce();

	// Camera
	UFUNCTION()
	void RotateCamera(FVector2D Input);
	UFUNCTION()
	FVector ToLocalSpace(const FVector& WorldSpace);

	UFUNCTION(BlueprintImplementableEvent)
	void PickupCollectable();

	// Hit reaction
	UFUNCTION(BlueprintCallable)
	void BeginHitReaction(float UnlockTime);
	UFUNCTION()
	void EndHitReaction();

	// Blueprint Events
	UFUNCTION(BlueprintImplementableEvent)
	void ReleaseJumpBP();
	UFUNCTION(BlueprintImplementableEvent)
	void StartDownforceBP();
	UFUNCTION(BlueprintImplementableEvent)
	void StopDownforceBP();
	UFUNCTION(BlueprintImplementableEvent)
	void BeginHitReactionBP(float UnlockTime);
	UFUNCTION(BlueprintImplementableEvent)
	void EndHitReactionBP();

protected:
	// Overrides
	virtual void BeginPlay() override;

	// Damage
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                  AController* InstigatedBy, AActor* DamageCauser);

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BlobMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* EyeLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* EyeRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* DynamicCamera;

	// Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

	// Movement - Tuning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementForce = 100000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxSpeed = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GroundFriction = 8.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AirFriction = 8.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AirControl = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MouseSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Movement")
	float TimeToStop = 1.0f;
	UPROPERTY(EditAnywhere, Category= "Movement|Jump")
	float GroundedDistance = 20.0f;

	// Jump/Charge - Tuning
	UPROPERTY(EditAnywhere, Category= "Movement|Jump")
	float MaxChargeTime = 5.0f;
	UPROPERTY(EditAnywhere, Category= "Movement|Jump")
	float MaxJumpVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float JumpForceScalar = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxZVelocity = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DownforceForce;
	UPROPERTY(EditAnywhere)
	float WallStuckCheckRange = 100.0f;
	UPROPERTY(EditAnywhere, Category= "Movement|Jump")
	float WallStuckCheckRadius = 50.0f;

	// Camera/Visuals - Tuning
	UPROPERTY(BlueprintReadOnly)
	double BasePitch;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Movement|Jump")
	double MaxPitch = 30.0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Movement|Jump")
	float MinZScale = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Movement|Jump")
	float MaxXYScale = 1.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Movement|Jump")
	FVector ChargedEyeScale = FVector(1.0f, 1.0f, 0.5f);

	// Animation - Runtime State
	UPROPERTY(BlueprintReadOnly, Category= "Animation")
	bool bIsChargingJump;
	UPROPERTY(BlueprintReadOnly, Category= "Animation")
	FVector SquishScale;
	UPROPERTY(BlueprintReadOnly, Category= "Animation")
	FVector BaseScale;

	// Gameplay - Runtime State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsGrounded;
	UPROPERTY(BlueprintReadOnly)
	float ChargeProgress;

	// Internal helpers/state
	UPROPERTY()
	FVector InputVec;
	float BaseMass;
	float DownforceMassMultiplier = 50.0f;
	FTimerHandle TH_BlockInput;
	FTimerHandle TimerHandle_LockVelocity;
	mutable FCriticalSection GroundedLock;

	// Timers / Callbacks
	void UnblockInput();

private:
	// Internal State
	bool bIsStunned = false;

	// Internal Logic
	void UpdateGrounded();
	void ApplyMovementForce(float DeltaTime);
	void RotateMesh(float DeltaTime) const;
	void LimitVelocity() const;
	bool CheckWallStuck(FVector MoveInput);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blob_ConveyorBelt.h"
#include "Engine/DecalActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blob_PlayerCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class BLOB_API ABlob_PlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlob_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category= "Movement|Jump")
	float MaxChargeTime = 5.0f;

	UPROPERTY(EditAnywhere, Category= "Movement|Jump")
	float MaxJumpVelocity;

	UPROPERTY(BlueprintReadOnly)
	bool bIsChargingJump;

	UPROPERTY(BlueprintReadOnly)
	FVector SquishScale;

	UPROPERTY(BlueprintReadOnly)
	FVector BaseScale;

	UPROPERTY(BlueprintReadOnly)
	double BasePitch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Movement|Jump")
	float MinZScale = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Movement|Jump")
	float MaxXYScale = 1.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Movement|Jump")
	double MaxPitch = 30.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Movement|Jump")
	FVector ChargedEyeScale = FVector(1.0f, 1.0f, 0.5f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Movement")
	float TimeToStop = 1.0f;

	UPROPERTY(BlueprintReadOnly)
	float ChargeProgress;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsGrounded;
	
	FVector InputVec;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float JumpForceScalar = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DownforceForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxZVelocity = 1000.0f;
	FTimerHandle TimerHandle_LockVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBlob_Settings* Settings;

public:	
	UFUNCTION(BlueprintCallable)
	UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }

	void ApplyGroundVelocity() const;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateChargeProgress(float DeltaTime);

	UFUNCTION()
	void ChargeJump();

	UPROPERTY(EditAnywhere, Category= "Movement|Jump")
	float GroundedDistance = 20.0f;

	UFUNCTION()
	void ReleaseJump();
	void CancelMove();

	UFUNCTION(BlueprintCallable)
	void LoadSettings();

	UFUNCTION(BlueprintImplementableEvent)
	void ReleaseJumpBP();
	void AddMovementInput(FVector NewInputVec);
	void StartDownforce();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDownforceBP();
	void StopDownforce();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StopDownforceBP();
	float BaseMass;
	float DownforceMassMultiplier = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

	UFUNCTION()
	void RotateCamera(FVector2D Input);

	UFUNCTION()
	FVector ToLocalSpace(FVector WorldSpace);

	UFUNCTION()
	void PickupBonusLife()
	{
		Lives++;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Lives = 3;

	UFUNCTION(BlueprintCallable)
	void UnlockVelocity(float UnlockTime);

	UPROPERTY(EditAnywhere)
	FVector GroundVelocity;

private:
	void UpdateGrounded();

	void ApplyMovementForce(float DeltaTime);
	void RotateMesh(float DeltaTime);
	void LimitVelocity();

	bool CheckWallStuck(FVector MoveInput);
	bool bUnlockedVelocity = false;

	UFUNCTION()
	void LockVelocity();
};

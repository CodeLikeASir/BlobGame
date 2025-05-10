// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Blob_PlayerCharacter.generated.h"

class ABlob_PlayerShadow;
class UInputAction;
class UInputMappingContext;

UCLASS()
class BLOB_API ABlob_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlob_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float MaxChargeTime = 5.0f;

	UPROPERTY(EditAnywhere)
	float MaxJumpVelocity;

	UPROPERTY(BlueprintReadOnly)
	bool bIsChargingJump;

	UPROPERTY(BlueprintReadOnly)
	FVector SquishScale;

	UPROPERTY(BlueprintReadOnly)
	FVector BaseScale;

	UPROPERTY(BlueprintReadOnly)
	double BasePitch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MinZScale = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxXYScale = 1.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	double MaxPitch = 30.0;

	UPROPERTY(BlueprintReadOnly)
	float ChargeProgress;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PlaceholderMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* EyeLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* EyeRight;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ChargeJump();

	UFUNCTION()
	void ReleaseJump();

	UFUNCTION(BlueprintImplementableEvent)
	void ReleaseJumpBP();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

private:
	// Bounce properties
	UPROPERTY(EditAnywhere, Category = "Movement|Bounce")
	float BounceFactor = 0.6f;
    
	UPROPERTY(EditAnywhere, Category = "Movement|Bounce")
	float BounceThreshold = 300.0f;
    
	// Function to handle bounce
	void HandleBounce(const FHitResult& Hit);

	UPROPERTY()
	ABlob_PlayerShadow* PlayerShadow;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABlob_PlayerShadow> PlayerShadowClass;

protected:
	// Override landing event
	virtual void Landed(const FHitResult& Hit) override;
};

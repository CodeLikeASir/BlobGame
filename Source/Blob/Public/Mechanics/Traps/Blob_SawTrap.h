// (C) Jan Meissner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_SawTrap.generated.h"

/* Spinning saw trap that follows a pre-determined path and has safe spots on the top center */
UCLASS()
class BLOB_API ABlob_SawTrap : public AActor
{
	GENERATED_BODY()

protected:
	/* COMPONENTS - START */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SawTrap")
	class USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SawTrap")
	UStaticMeshComponent* SawBaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SawTrap")
	UStaticMeshComponent* SawBladeMesh;
	/* COMPONENTS - END */

	/* Movement speed along pre-determined path */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.1"))
	float MovementSpeed = 100.0f;

	/* If spline is a circle OR movement is between start and end positions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bShouldLoop = true;

	/* Allows for (dynamically) switch move direction along path */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bReverseDirection = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bRotateAlongSpline = false;

	/* Degrees per second for saw blade rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 360.0f;
	
	/* How long the player is stunned when colliding with spikes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SawTrap")
	float StunLength = 0.3f;

	/* How hard the pushback is when player hits spikes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SawTrap")
	int StunForceMultiplier = 1000.f;

	/* Additional up force so that player will always get pushed away AND UP on hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SawTrap")
	FVector StunUpForce = FVector(0.f, 0.f, 1000.f);

private:
	/* Current distance along the spline */
	float CurrentDistance = 0.0f;

	/* Direction multiplier for movement */
	float DirectionMultiplier = 1.0f;

	/* Total spline length */
	float SplineLength = 0.0f;

	/* Current rotation for the saw blade */
	float CurrentRotation = 0.0f;

public:
	ABlob_SawTrap();

protected:
	UFUNCTION()
	void DamageActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                 FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMovementSpeed(float NewSpeed);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ReverseMovementDirection();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetPosition();

	UFUNCTION(BlueprintPure, Category = "Movement")
	float GetCurrentDistanceAlongSpline() const { return CurrentDistance; }

	UFUNCTION(BlueprintPure, Category = "Movement")
	float GetSplineLength() const { return SplineLength; }
};

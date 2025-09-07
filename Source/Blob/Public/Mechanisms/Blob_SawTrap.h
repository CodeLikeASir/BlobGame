// (C) Jan Meissner

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_SawTrap.generated.h"

UCLASS()
class BLOB_API ABlob_SawTrap : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SawTrap")
	class USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SawTrap")
	UStaticMeshComponent* SawBaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SawTrap")
	UStaticMeshComponent* SawBladeMesh;

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.1"))
	float MovementSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bShouldLoop = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bReverseDirection = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bRotateAlongSpline = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 360.0f; // Degrees per second for saw blade rotation

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SawTrap")
	float StunLength = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SawTrap")
	int StunForceMultiplier = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SawTrap")
	FVector StunUpForce = FVector(0.f, 0.f, 1000.f);

private:
	// Current distance along the spline
	float CurrentDistance = 0.0f;

	// Direction multiplier for movement
	float DirectionMultiplier = 1.0f;

	// Total spline length
	float SplineLength = 0.0f;

	// Current rotation for the saw blade
	float CurrentRotation = 0.0f;

public:
	// Sets default values for this actor's properties
	ABlob_SawTrap();

protected:
	UFUNCTION()
	void DamageActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                 FVector NormalImpulse, const FHitResult& Hit);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Utility functions
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

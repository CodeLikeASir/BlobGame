// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_PlayerCamera.generated.h"

class ABlob_PlayerCharacter;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class BLOB_API ABlob_PlayerCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlob_PlayerCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* ArmRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	float CameraTargetOffset = 100.0f;

	UPROPERTY(EditAnywhere)
	float Sensitivity = 1000.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RotateCamera(FVector2D Value);
	void MoveCameraUpDown(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ABlob_PlayerCharacter* PlayerCharacter;

	UFUNCTION(BlueprintCallable)
	void ResetCamera();

	FRotator GetCurrentRotation();

	UFUNCTION(BlueprintCallable)
	FVector GetCameraLocation();
};

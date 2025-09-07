#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_SpringPad.generated.h"

class UPhysicsConstraintComponent;

UCLASS()
class BLOB_API ABlob_SpringPad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlob_SpringPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseIndicator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* SpringPad;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPhysicsConstraintComponent* SpringConstraint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaunchThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaunchForce;

	bool bCanLaunch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TraceRadius;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Launch();
};

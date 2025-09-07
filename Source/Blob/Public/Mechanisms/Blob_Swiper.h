#pragma once

#include "CoreMinimal.h"
#include "Blob_Toggleable.h"
#include "GameFramework/Actor.h"
#include "Blob_Swiper.generated.h"

UCLASS()
class BLOB_API ABlob_Swiper : public AActor, public IBlob_Toggleable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Swiper")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Swiper")
	UStaticMeshComponent* SwiperMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Swiper")
	class URotatingMovementComponent* RotatingMovementComponent;

public:
	// Sets default values for this actor's properties
	ABlob_Swiper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFullRotation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "!bFullRotation"), Category="Swiper")
	float MinAngle = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "!bFullRotation"), Category="Swiper")
	float MaxAngle = 0.0f;

	UPROPERTY(VisibleAnywhere)
	bool bIsOn = true;

	FRotator RotationSpeed;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnToggleOn_Implementation() override;
	virtual void OnToggleOff_Implementation() override;
};

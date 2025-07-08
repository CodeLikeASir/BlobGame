// (C) Jan Meissner

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
	
public:	
	// Sets default values for this actor's properties
	ABlob_Swiper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip = "Rotation (in degrees) per second"), Category="Swiper")
	float RotationSpeed = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RotationAxis = FVector(0.0f, 1.0f, 0.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsOn = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnToggleOn_Implementation() override;
	virtual void OnToggleOff_Implementation() override;
};

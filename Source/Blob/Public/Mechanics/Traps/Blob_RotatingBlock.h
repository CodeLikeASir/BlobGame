#pragma once

#include "CoreMinimal.h"
#include "Mechanics/Interactable/Blob_Toggleable.h"
#include "GameFramework/Actor.h"
#include "Blob_RotatingBlock.generated.h"

class UBoxComponent;

UCLASS()
class BLOB_API ABlob_RotatingBlock : public AActor, public IBlob_Toggleable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlob_RotatingBlock();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* SpikeCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StunLength = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StunForceMultiplier = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StunUpForce = FVector(0.f, 0.f, 1000.f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void DamageActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                 FVector NormalImpulse, const FHitResult& Hit);
};

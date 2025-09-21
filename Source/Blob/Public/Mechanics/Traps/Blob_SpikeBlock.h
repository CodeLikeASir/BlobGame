#pragma once

#include "CoreMinimal.h"
#include "Mechanics/Interactable/Blob_Toggleable.h"
#include "GameFramework/Actor.h"
#include "Blob_SpikeBlock.generated.h"

class UBoxComponent;

/* Spike block trap that continuously rotates but can be stopped via a connected ToggleTrigger */
UCLASS()
class BLOB_API ABlob_SpikeBlock : public AActor, public IBlob_Toggleable
{
	GENERATED_BODY()
	
protected:
	/* COMPONENTS - START */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* SpikeCollision;
	/* COMPONENTS - END */
	
public:
	ABlob_SpikeBlock();

	/* How long the player is stunned when colliding with spikes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StunLength = 0.3f;

	/* How hard the pushback is when player hits spikes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StunForceMultiplier = 1000.f;

	/* Additional up force so that player will always get pushed away AND UP on hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StunUpForce = FVector(0.f, 0.f, 1000.f);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/* Triggered on player being hit */
	UFUNCTION()
	void DamageActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                 FVector NormalImpulse, const FHitResult& Hit);
};

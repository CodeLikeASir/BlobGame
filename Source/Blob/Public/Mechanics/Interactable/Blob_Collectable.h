#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_Collectable.generated.h"

class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickupDelegate, int, Index);

/*
 * Optional collectable item that can be picked up by the player
 */
UCLASS()
class BLOB_API ABlob_Collectable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlob_Collectable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* PickupSphere;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	FPickupDelegate OnPickup;
};

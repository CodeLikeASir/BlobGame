

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_Checkpoint.generated.h"

class ABlob_CheckpointManager;

UCLASS()
class BLOB_API ABlob_Checkpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlob_Checkpoint();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int CheckpointIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ABlob_CheckpointManager* CheckpointManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CheckpointMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	bool bUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* UnlockedMesh;

	UFUNCTION(BlueprintImplementableEvent)
	void OnCheckpointReached();
};

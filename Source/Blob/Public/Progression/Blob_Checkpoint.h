#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_Checkpoint.generated.h"

class ABlob_CheckpointManager;
class ABlob_PlayerCharacter;

/*
 * Saves player progress on being reached as well as loading next sub-level and updating section time records
 */
UCLASS()
class BLOB_API ABlob_Checkpoint : public AActor
{
	GENERATED_BODY()

public:
	ABlob_Checkpoint();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int CheckpointIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ABlob_CheckpointManager* CheckpointManager;

protected:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CheckpointMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void SetUnlockedMesh();

	UPROPERTY()
	bool bUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* UnlockedMesh;

public:
	UFUNCTION()
	void OnCheckpointReached(ABlob_PlayerCharacter* Player);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCheckpointReachedBP();
};

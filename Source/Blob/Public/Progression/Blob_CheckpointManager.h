#pragma once

#include "CoreMinimal.h"
#include "Blob_Checkpoint.h"
#include "GameFramework/Actor.h"
#include "Core/Blob_PlayerController.h"
#include "Templates/Tuple.h"
#include "Blob_CheckpointManager.generated.h"

UENUM(BlueprintType)
enum class EReachedResult : uint8
{
	OldCheckpoint,
	NewCheckpoint,
	GameEnd,
	InvalidCheckpoint
};

/*
 * Keeps track of all active checkpoints and monitors player progress
 * 
 */
UCLASS()
class BLOB_API ABlob_CheckpointManager : public AActor
{
	GENERATED_BODY()

public:
	ABlob_CheckpointManager();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void CheckPlayerPosition();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "CheckpointManager")
	int CurrentCheckpointIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CheckpointManager")
	TMap<int, TSoftObjectPtr<ABlob_Checkpoint>> Checkpoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CheckpointManager")
	TArray<TSoftObjectPtr<UWorld>> Stages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CheckpointManager")
	TArray<FName> StageNames;

	TArray<int> LoadedStages;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "CheckpointManager")
	TSoftObjectPtr<ABlob_Checkpoint> CurrentCheckpoint;

	int SpawnCheckpoint = 0;
	FTimerHandle TH_PlayerCheck;
	float PlayerResetThreshold = 500.0f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CheckpointManager")
	ABlob_PlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CheckpointManager")
	ABlob_PlayerController* PlayerController;

	UFUNCTION(BlueprintCallable, Category="CheckpointManager",
		meta=(ToolTip="Returns true if new checkpoint has been reached"))
	EReachedResult OnCheckpointReached(TSoftObjectPtr<ABlob_Checkpoint> Checkpoint, int CheckpointIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCheckpointReachedBP(int CheckpointIndex);

	UFUNCTION(BlueprintCallable)
	FTransform GetCheckpointTransform()
	{
		if (!CurrentCheckpoint.IsValid())
		{
			return FTransform::Identity;
		}

		return CurrentCheckpoint->GetActorTransform();
	}

	UFUNCTION(BlueprintCallable)
	void UnlockCheckpoints(int LastUnlockedIndex);

	UFUNCTION()
	void RegisterCheckpoint(ABlob_Checkpoint* Checkpoint);

	UFUNCTION()
	void UnregisterCheckpoint(const ABlob_Checkpoint* Checkpoint);
};

#pragma once

#include "CoreMinimal.h"
#include "Blob_Checkpoint.h"
#include "GameFramework/Actor.h"
#include "Player/Blob_PlayerController.h"
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

UCLASS()
class BLOB_API ABlob_CheckpointManager : public AActor
{
	GENERATED_BODY()

public:
	ABlob_CheckpointManager();

protected:
	UFUNCTION()
	void CheckPlayerPosition();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "CheckpointManager")
	int CurrentCheckpointIndex = -1;

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
	FVector GetCheckpointLocation()
	{
		if (!CurrentCheckpoint.IsValid())
		{
			return FVector::ZeroVector;
		}

		return CurrentCheckpoint->GetActorLocation();
	}

	UFUNCTION(BlueprintCallable)
	FRotator GetCheckpointRotation()
	{
		if (!CurrentCheckpoint.IsValid())
		{
			return FRotator::ZeroRotator;
		}

		return CurrentCheckpoint->GetActorRotation();
	}

	UFUNCTION()
	void OnLevelLoaded();

	UFUNCTION(BlueprintCallable)
	void UnlockCheckpoints(int LastUnlockedIndex);
};

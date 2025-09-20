#include "Progression/Blob_CheckpointManager.h"

#include "Core/Blob_PlayerCharacter.h"
#include "Core/Blob_PlayerController.h"
#include "Progression/Blob_Checkpoint.h"

#include "WorldPartition/WorldPartitionLevelStreamingDynamic.h"

ABlob_CheckpointManager::ABlob_CheckpointManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABlob_CheckpointManager::CheckPlayerPosition()
{
	if (CurrentCheckpoint == nullptr || PlayerCharacter == nullptr || PlayerController == nullptr)
	{
		return;
	}

	float ZCheckpoint = CurrentCheckpoint->GetActorLocation().Z;
	if (float ZPlayer = PlayerCharacter->GetActorLocation().Z;
		ZCheckpoint - ZPlayer >= PlayerResetThreshold)
	{
		PlayerController->ShowResetNotification();
	}
	else
	{
		PlayerController->ClearNotifications();
	}
}

void ABlob_CheckpointManager::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TH_PlayerCheck, this, &ABlob_CheckpointManager::CheckPlayerPosition, 0.5f, true);
}

EReachedResult ABlob_CheckpointManager::OnCheckpointReached(TSoftObjectPtr<ABlob_Checkpoint> Checkpoint,
                                                            int CheckpointIndex)
{
	if (CheckpointIndex < 0)
		return EReachedResult::InvalidCheckpoint;

	if (CheckpointIndex >= Stages.Num())
		return EReachedResult::GameEnd;

	CurrentCheckpointIndex = CheckpointIndex;
	CurrentCheckpoint = Checkpoint;

	auto Stage = Stages[CheckpointIndex];

	// Don't load already loaded levels
	if (LoadedStages.Contains(CheckpointIndex))
		return EReachedResult::OldCheckpoint;

	bool bSuccess;

	ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), Stage, FTransform::Identity, bSuccess);
	OnCheckpointReachedBP(CheckpointIndex);
	LoadedStages.Add(CheckpointIndex);

	return EReachedResult::NewCheckpoint;
}

void ABlob_CheckpointManager::UnlockCheckpoints(int LastUnlockedIndex)
{
	int LastIndex = FMath::Min(LastUnlockedIndex, Stages.Num() - 1);
	CurrentCheckpointIndex = LastIndex;
	for (int Index = 0; Index <= LastIndex; Index++)
	{
		auto Stage = Stages[Index];
		if (LoadedStages.Contains(Index))
		{
			continue;
		}

		bool bSuccess;
		LoadedStages.Add(Index);
		ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
			GetWorld(), Stage, FTransform::Identity, bSuccess);
	}
}

void ABlob_CheckpointManager::RegisterCheckpoint(ABlob_Checkpoint* Checkpoint)
{
	if (Checkpoint == nullptr)
		return;

	if (Checkpoints.Contains(Checkpoint->CheckpointIndex))
	{
		Checkpoints.Remove(Checkpoint->CheckpointIndex);
	}

	Checkpoints.Add(Checkpoint->CheckpointIndex, Checkpoint);

	// Save as current checkpoint if index is correct
	if (CurrentCheckpointIndex == Checkpoint->CheckpointIndex)
	{
		CurrentCheckpoint = Checkpoint;
	}
}

void ABlob_CheckpointManager::UnregisterCheckpoint(const ABlob_Checkpoint* Checkpoint)
{
	if (Checkpoints.Contains(Checkpoint->CheckpointIndex))
	{
		Checkpoints.Remove(Checkpoint->CheckpointIndex);
	}
}

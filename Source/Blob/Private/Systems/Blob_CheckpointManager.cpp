#include "Systems/Blob_CheckpointManager.h"

#include "Blob_Checkpoint.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Blob_PlayerController.h"
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

	ULevelStreamingDynamic* StreamingLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
		GetWorld(), Stage, FTransform::Identity, bSuccess);

	if (Checkpoint == nullptr && StreamingLevel)
	{
		// Create a delegate to handle when the level finishes loading
		StreamingLevel->OnLevelLoaded.AddDynamic(this, &ABlob_CheckpointManager::OnLevelLoaded);
	}

	UE_LOG(LogTemp, Log, TEXT("Loaded Level for Checkpoint %d successfully? %d"), CheckpointIndex, bSuccess);
	OnCheckpointReachedBP(CheckpointIndex);
	LoadedStages.Add(CheckpointIndex);

	return EReachedResult::NewCheckpoint;
}

void ABlob_CheckpointManager::OnLevelLoaded()
{
	// Make sure the level is fully loaded
	if (UWorld* World = GetWorld())
	{
		// Find all checkpoints in the loaded level
		TArray<AActor*> FoundCheckpoints;
		UGameplayStatics::GetAllActorsOfClass(World, ABlob_Checkpoint::StaticClass(), FoundCheckpoints);

		// Process found checkpoints
		for (AActor* Actor : FoundCheckpoints)
		{
			if (ABlob_Checkpoint* CheckpointActor = Cast<ABlob_Checkpoint>(Actor))
			{
				if (CurrentCheckpoint == nullptr)
					CurrentCheckpoint = CheckpointActor;
				else
					CurrentCheckpoint = CheckpointActor->CheckpointIndex > CurrentCheckpoint->CheckpointIndex
						                    ? CheckpointActor
						                    : CurrentCheckpoint;

				UE_LOG(LogTemp, Log, TEXT("Found checkpoint in loaded level: %s (index: %d)"),
				       *CheckpointActor->GetName(), CheckpointActor->CheckpointIndex);
			}
		}
	}
}

void ABlob_CheckpointManager::UnlockCheckpoints(int LastUnlockedIndex)
{
	int LastIndex = FMath::Min(LastUnlockedIndex, Stages.Num() - 1);
	for (int Index = 0; Index <= LastIndex; Index++)
	{
		auto Stage = Stages[Index];
		if (LoadedStages.Contains(Index))
			continue;

		bool bSuccess;
		LoadedStages.Add(Index);
		ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
			GetWorld(), Stage, FTransform::Identity, bSuccess);
	}
}




#include "Blob_CheckpointManager.h"

#include "Blob_Checkpoint.h"
#include "Kismet/GameplayStatics.h"
#include "WorldPartition/WorldPartitionLevelStreamingDynamic.h"

// Sets default values
ABlob_CheckpointManager::ABlob_CheckpointManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABlob_CheckpointManager::BeginPlay()
{
	Super::BeginPlay();
}

bool ABlob_CheckpointManager::OnCheckpointReached(TSoftObjectPtr<ABlob_Checkpoint> Checkpoint, int CheckpointIndex)
{
	if (CheckpointIndex < 0)
		return false;
	
	CurrentCheckpointIndex = CheckpointIndex;
	CurrentCheckpoint = Checkpoint;
	
	auto Stage = Stages[CheckpointIndex];
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
	
	return true;
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
				// Here you have access to the checkpoint actor in the loaded level
				// You can process it as needed
				UE_LOG(LogTemp, Log, TEXT("Found checkpoint in loaded level: %s"), *CheckpointActor->GetName());
                
				// Store reference if needed
				CurrentCheckpoint = CheckpointActor;
				break;
			}
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Blob_CheckpointManager.h"

#include "Blob_Checkpoint.h"
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

bool ABlob_CheckpointManager::CheckpointReached(TSoftObjectPtr<ABlob_Checkpoint> Checkpoint, int CheckpointIndex)
{
	CurrentCheckpointIndex = CheckpointIndex;
	CurrentCheckpoint = Checkpoint;
	OnCheckpointReached(CheckpointIndex);
	return true;
}

void ABlob_CheckpointManager::OnCheckpointReached(int CheckpointIndex)
{
	if (CheckpointIndex < 0)
		return;
	
	auto Stage = Stages[CheckpointIndex];
	bool bSuccess;
	ULevelStreamingDynamic* StreamingLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
		GetWorld(), Stage, FTransform::Identity, bSuccess);
	UE_LOG(LogTemp, Log, TEXT("Loaded Level for Checkpoint %d successfully? %d"), CheckpointIndex, bSuccess);
}

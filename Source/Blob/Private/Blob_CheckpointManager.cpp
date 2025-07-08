// Fill out your copyright notice in the Description page of Project Settings.


#include "Blob_CheckpointManager.h"

#include "Blob_Checkpoint.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
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

bool ABlob_CheckpointManager::CheckpointReached(int CheckpointIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
	                                 FString::Printf(
		                                 TEXT("Checkpoint %d reached (previously %d)"), CheckpointIndex,
		                                 CurrentCheckpointIndex));
	if (CurrentCheckpointIndex >= CheckpointIndex)
		return false;

	OnCheckpointReached(CheckpointIndex);
	CurrentCheckpointIndex = CheckpointIndex;
	return true;
}

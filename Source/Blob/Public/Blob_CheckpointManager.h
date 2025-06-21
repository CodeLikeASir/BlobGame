// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SoftWorldReference.h"
#include "GameFramework/Actor.h"
#include "Blob_CheckpointManager.generated.h"

UCLASS()
class BLOB_API ABlob_CheckpointManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlob_CheckpointManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CheckpointManager")
	TArray<class ABlob_Checkpoint*> Checkpoints;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "CheckpointManager")
	int CurrentCheckpointIndex = -1;

public:
	UFUNCTION(BlueprintCallable, Category="CheckpointManager", meta=(ToolTip="Returns true if new checkpoint has been reached"))
	bool CheckpointReached(int CheckpointIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCheckpointReached(int CheckpointIndex);

	UFUNCTION(BlueprintCallable)
	ABlob_Checkpoint* GetCurrentCheckpoint() { return Checkpoints[CurrentCheckpointIndex]; }
};

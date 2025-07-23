

#pragma once

#include "CoreMinimal.h"
#include "Blob_Checkpoint.h"
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

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "CheckpointManager")
	int CurrentCheckpointIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CheckpointManager")
	TArray<TSoftObjectPtr<UWorld>> Stages;

	TSoftObjectPtr<ABlob_Checkpoint> CurrentCheckpoint;

public:
	UFUNCTION(BlueprintCallable, Category="CheckpointManager", meta=(ToolTip="Returns true if new checkpoint has been reached"))
	bool CheckpointReached(TSoftObjectPtr<ABlob_Checkpoint> Checkpoint, int CheckpointIndex);

	UFUNCTION()
	void OnCheckpointReached(int CheckpointIndex);

	UFUNCTION(BlueprintCallable)
	FVector GetCheckpointLocation()
	{
		return CurrentCheckpoint->GetActorLocation();
	}
};

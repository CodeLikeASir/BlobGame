#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Blob_Savegame.generated.h"

/*
 * Saves player progress (highest reached checkpoint) as well as all achieved (speedrun) times
 */
UCLASS()
class BLOB_API UBlob_Savegame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LastCheckpointIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LastCheckpointTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, float> CheckpointTimes;
};

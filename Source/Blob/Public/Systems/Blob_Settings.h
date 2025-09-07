#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Templates/Tuple.h"
#include "Blob_Settings.generated.h"

// Define a reflected struct for leaderboard entries
USTRUCT(BlueprintType)
struct FLeaderboardEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite)
	int HighestCheckpoint;

	UPROPERTY(BlueprintReadWrite)
	TArray<float> CheckpointTimes;

	UPROPERTY(BlueprintReadWrite)
	bool bIsFinished;

	UPROPERTY(BlueprintReadOnly)
	float TotalTime = 0.f;
};

/**
 * 
 */
UCLASS()
class BLOB_API UBlob_Settings : public USaveGame
{
	GENERATED_BODY()

public:
	UBlob_Settings();

	/* Gameplay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowSpeedrunTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLeaderboardEntry> Leaderboard;

	/* Controls */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraSensitivity;

	/* Audio */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MasterVolume;

	UFUNCTION(BlueprintCallable)
	void AddLeaderboardTime(FString Name, float Time);

	UFUNCTION(BlueprintCallable)
	TArray<FLeaderboardEntry> GetFilteredLeaderboard(int CheckpointIndex);
};

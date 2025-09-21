#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blob_GameInstance.generated.h"

class UBlob_Settings;
class UBlob_Savegame;

/**
 * Implements core game instance functionality, such as savegame handling and session timing.
 * 
 * Provides savegame management, session timer control, and utility methods to interact with and manage the game state
 */
UCLASS()
class BLOB_API UBlob_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY()
	UBlob_Savegame* CurrentSavegame;

	UPROPERTY()
	double SessionStartTime;

	UPROPERTY()
	double PreviousSessionTime;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Savegame")
	FString SavegameName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Savegame")
	int32 SavegameUserIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	FString SettingsName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	int32 SettingsUserIndex = 0;

public:
	/* Retrieves existing savegame or creates a new one if none were found */
	UFUNCTION(BlueprintCallable, Category = "Savegame")
	UBlob_Savegame* GetSavegame();

	/* Checks if a previous savegame exists */
	UFUNCTION(BlueprintCallable, Category = "Savegame")
	bool IsFirstSession();

	/* Save the current savegame to disk */
	UFUNCTION(BlueprintCallable, Category = "Savegame")
	void SaveSavegame();

	/* Starts the timer tracking total playthrough playtime */
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartTimer();

	/* Pauses timer progression */
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void PauseTimer();

	/* Retrieves current playthrough playtime */
	UFUNCTION(BlueprintCallable, Category = "Timer")
	double GetCurrentTime() const;
};

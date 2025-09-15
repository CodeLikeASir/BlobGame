// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blob_GameInstance.generated.h"

class UBlob_Settings;
class UBlob_Savegame;
/**
 * 
 */
UCLASS()
class BLOB_API UBlob_GameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
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

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Timer")
	bool bIsTimerRunning;

public:
	UFUNCTION(BlueprintCallable, Category = "Savegame")
	UBlob_Savegame* GetSavegame();

	UFUNCTION(BlueprintCallable, Category = "Savegame")
	bool IsFirstSession();

	UFUNCTION(BlueprintCallable, Category = "Savegame")
	void SaveSavegame();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartTimer();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void PauseTimer();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	float GetCurrentTime() const;
};

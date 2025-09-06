#include "Blob_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Blob_Savegame.h"
#include "Systems/Blob_Settings.h"

UBlob_Savegame* UBlob_GameInstance::GetSavegame()
{
	if (CurrentSavegame == nullptr)
	{
		// Check if a previous savegame exists
		UBlob_Savegame* LoadedSave =
			Cast<UBlob_Savegame>(UGameplayStatics::LoadGameFromSlot(SavegameName, SavegameUserIndex));
		if (LoadedSave)
		{
			CurrentSavegame = LoadedSave;
		}
		else
		{
			CurrentSavegame =
				Cast<UBlob_Savegame>(UGameplayStatics::CreateSaveGameObject(UBlob_Savegame::StaticClass()));
		}
	}

	return CurrentSavegame;
}

bool UBlob_GameInstance::IsFirstSession()
{
	return GetSavegame()->LastCheckpointIndex >= 0;
}

void UBlob_GameInstance::SaveSavegame()
{
	UGameplayStatics::SaveGameToSlot(CurrentSavegame, SavegameName, SavegameUserIndex);
}

UBlob_Settings* UBlob_GameInstance::GetCustomSettings()
{
	if (CurrentSavegame == nullptr)
	{
		// Check if a previous savegame exists
		UBlob_Settings* LoadedSave =
			Cast<UBlob_Settings>(UGameplayStatics::LoadGameFromSlot(SettingsName, SettingsUserIndex));
		if (LoadedSave)
		{
			CurrentSettings = LoadedSave;
		}
		else
		{
			CurrentSettings =
				Cast<UBlob_Settings>(UGameplayStatics::CreateSaveGameObject(UBlob_Settings::StaticClass()));
		}
	}

	return CurrentSettings;
}

void UBlob_GameInstance::SaveCustomSettings()
{
	UGameplayStatics::SaveGameToSlot(CurrentSettings, SettingsName, SettingsUserIndex);
}

void UBlob_GameInstance::StartTimer()
{
	bIsTimerRunning = true;
	SessionStartTime = GetWorld()->GetTimeSeconds();
}

void UBlob_GameInstance::PauseTimer()
{
	bIsTimerRunning = false;
	PreviousSessionTime = GetCurrentTime();
}

float UBlob_GameInstance::GetCurrentTime() const
{
	return GetWorld()->GetTimeSeconds() - SessionStartTime + PreviousSessionTime;
}
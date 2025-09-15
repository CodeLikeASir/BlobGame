#include "Blob_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Blob_Savegame.h"

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
	return GetSavegame()->LastCheckpointIndex < 0;
}

void UBlob_GameInstance::SaveSavegame()
{
	UGameplayStatics::SaveGameToSlot(CurrentSavegame, SavegameName, SavegameUserIndex);
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

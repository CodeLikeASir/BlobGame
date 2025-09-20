#include "Core/Blob_GameInstance.h"

#include "Progression/Blob_Savegame.h"

#include "Kismet/GameplayStatics.h"

UBlob_Savegame* UBlob_GameInstance::GetSavegame()
{
	if (CurrentSavegame == nullptr)
	{
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
	// If a savegame without any progress was returned, it has to be either newly created OR
	// no relevant progress was made in earlier plays 
	return GetSavegame()->LastCheckpointIndex < 0;
}

void UBlob_GameInstance::SaveSavegame()
{
	UGameplayStatics::SaveGameToSlot(CurrentSavegame, SavegameName, SavegameUserIndex);
}

void UBlob_GameInstance::StartTimer()
{
	SessionStartTime = GetWorld()->GetTimeSeconds();
}

void UBlob_GameInstance::PauseTimer()
{
	PreviousSessionTime = GetCurrentTime();
}

double UBlob_GameInstance::GetCurrentTime() const
{
	return GetWorld()->GetTimeSeconds() - SessionStartTime + PreviousSessionTime;
}

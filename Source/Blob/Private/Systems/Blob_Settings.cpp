


#include "Systems/Blob_Settings.h"
#include "Templates/Tuple.h"

UBlob_Settings::UBlob_Settings()
{
	bShowSpeedrunTimer = false;
	CameraSensitivity = 1.0f;
	MasterVolume = 100.0f;
}

void UBlob_Settings::AddLeaderboardTime(FString Name, float Time)
{
	FLeaderboardEntry NewEntry;
	NewEntry.PlayerName = Name;
	NewEntry.CheckpointTimes.Add(Time);
	NewEntry.HighestCheckpoint = Leaderboard.Num();
}

TArray<FLeaderboardEntry> UBlob_Settings::GetFilteredLeaderboard(int CheckpointIndex)
{
	TArray<TPair<FString, float>> SortedLeaderboard;
	SortedLeaderboard.Reserve(Leaderboard.Num());

	for (auto Entry : Leaderboard)
	{
		if (Entry.HighestCheckpoint < CheckpointIndex)
			continue;

		float RelevantTime = Entry.CheckpointTimes[CheckpointIndex];
		SortedLeaderboard.Emplace(Entry.PlayerName, RelevantTime);
	}

	/*
	// Sort by time (value) - lowest first
	SortedLeaderboard.Sort(
		[CheckpointIndex](const FLeaderboardEntry& A, const FLeaderboardEntry& B)
	{
		return A.CheckpointTimes[CheckpointIndex] < B.CheckpointTimes[CheckpointIndex];
	});
	*/

	TArray<FLeaderboardEntry> Out;
	Out.Reserve(SortedLeaderboard.Num());

	for (const TPair<FString, float>& Entry : SortedLeaderboard)
	{
		FLeaderboardEntry AddedEntry;
		AddedEntry.PlayerName = Entry.Key;
		Out.Emplace(Entry.Key, Entry.Value);
	}

	return Out;
}
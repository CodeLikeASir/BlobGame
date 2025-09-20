#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blob_GameMode.generated.h"

class UBlob_Savegame;

/**
 * @class ABlob_GameMode
 * @brief A custom game mode class for the game.
 *
 * Currently only used to allow for configuring default classes for PlayerController, Pawn and GameInstance.
 */
UCLASS()
class BLOB_API ABlob_GameMode : public AGameModeBase
{
	GENERATED_BODY()
};

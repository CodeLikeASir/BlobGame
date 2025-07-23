

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Blob_Settings.generated.h"

/**
 * 
 */
UCLASS()
class BLOB_API UBlob_Settings : public USaveGame
{
	GENERATED_BODY()

public:
	UBlob_Settings();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraSensitivity;
};

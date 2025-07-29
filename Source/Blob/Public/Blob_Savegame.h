// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Blob_Savegame.generated.h"

/**
 * 
 */
UCLASS()
class BLOB_API UBlob_Savegame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LastCheckpointIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LastCheckpointTime;
};

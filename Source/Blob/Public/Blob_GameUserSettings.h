// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Blob_GameUserSettings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class BLOB_API UBlob_GameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UBlob_GameUserSettings();
	
	/* Gameplay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHardcoreMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowSpeedrunTimer;
	
	/* Controls */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraSensitivity;

	UFUNCTION(BlueprintCallable)
	static UBlob_GameUserSettings* GetCustomSettings();

	/* Audio */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MasterVolume;
};

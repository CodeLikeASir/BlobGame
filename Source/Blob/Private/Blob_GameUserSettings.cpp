// Fill out your copyright notice in the Description page of Project Settings.


#include "Blob_GameUserSettings.h"

UBlob_GameUserSettings::UBlob_GameUserSettings()
{
	bIsHardcoreMode = false;
	bShowSpeedrunTimer = false;
	MasterVolume = 1.f;
	CameraSensitivity = 1.f;
}

UBlob_GameUserSettings* UBlob_GameUserSettings::GetCustomSettings()
{
	return Cast<UBlob_GameUserSettings>(GetGameUserSettings());
}

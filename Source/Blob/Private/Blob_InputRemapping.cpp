// Fill out your copyright notice in the Description page of Project Settings.

#include "Blob_InputRemapping.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "InputCoreTypes.h"
#include "Misc/EnumRange.h"
#include "UObject/Object.h"
#include "UserSettings/EnhancedInputUserSettings.h"

UBlob_InputRemapping::UBlob_InputRemapping(): UserSettings(nullptr)
{
}

UEnhancedInputUserSettings* UBlob_InputRemapping::Init(UEnhancedInputLocalPlayerSubsystem* Subsystem,
                                                       UInputMappingContext* BasicContext)
{
	auto Settings = Subsystem->GetUserSettings();
	if (Settings == nullptr)
	{
		Subsystem->InitalizeUserSettings();
		Settings = Subsystem->GetUserSettings();
	}

	Settings->RegisterInputMappingContext(BasicContext);
	UserSettings = Settings;
	return Settings;
}

void UBlob_InputRemapping::RemapKey(FName ActionMappingName, FKey NewKey)
{
	FMapPlayerKeyArgs Args = {};
	Args.MappingName = ActionMappingName;
	Args.Slot = EPlayerMappableKeySlot::First;
	Args.NewKey = NewKey;
	
	// If you want to, you can additionally specify this mapping to only be applied to a certain hardware device or key profile
	//Args.ProfileId =
	//Args.HardwareDeviceId =
	
	if (UserSettings)
	{
		FGameplayTagContainer FailureReason;
		UserSettings->MapPlayerKey(Args, FailureReason);
		if (!FailureReason.IsEmpty())
		{
		UE_LOG(LogTemp, Error, TEXT("[InputRemapping] Remapping key failed due to %s"), *FailureReason.ToString()); 
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[InputRemapping] UserSettings is null")); 
	}
}

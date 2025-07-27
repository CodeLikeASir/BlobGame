// Fill out your copyright notice in the Description page of Project Settings.


#include "Blob_BlueprintFunctionLibrary.h"

FText UBlob_BlueprintFunctionLibrary::FormatTime(float TimeInSeconds)
{
    const float ClampedTime = FMath::Max(0.0f, TimeInSeconds);
    
    const int32 Minutes = FMath::FloorToInt(ClampedTime / 60.0f);
    const float RemainingSeconds = ClampedTime - Minutes * 60.0f;
    const int32 Seconds = FMath::FloorToInt(RemainingSeconds);
    const int32 Milliseconds = FMath::FloorToInt((RemainingSeconds - Seconds) * 1000.0f);

    return FText::FromString(FString::Printf(TEXT("%02d:%02d.%03d"), Minutes, Seconds, Milliseconds));
}
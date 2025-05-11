// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Blob_Toggleable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UBlob_Toggleable : public UInterface
{
	GENERATED_BODY()
};
 
/* Actual Interface declaration. */
class IBlob_Toggleable
{
	GENERATED_BODY()
 
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnToggleOn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnToggleOff();
};
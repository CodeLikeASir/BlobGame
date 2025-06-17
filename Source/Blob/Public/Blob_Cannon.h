// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_Cannon.generated.h"

UCLASS()
class BLOB_API ABlob_Cannon : public AActor
{
	GENERATED_BODY()
	
public:
#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintImplementableEvent)
	void OnComponentMoved();
};

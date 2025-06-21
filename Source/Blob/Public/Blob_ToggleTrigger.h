// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blob_Toggleable.h"
#include "GameFramework/Actor.h"
#include "Blob_ToggleTrigger.generated.h"

UCLASS()
class BLOB_API ABlob_ToggleTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlob_ToggleTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Toggle")
	TArray<TSoftObjectPtr<AActor>> Toggleables;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Toggle")
	TArray<TSoftObjectPtr<AActor>> AltToggleables;

	UFUNCTION(BlueprintCallable)
	void OnToggleOn();

	UFUNCTION(BlueprintCallable)
	void OnToggleOff();
};

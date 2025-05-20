// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_PipeManager.generated.h"

class USplineMeshComponent;

UCLASS()
class BLOB_API ABlob_PipeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlob_PipeManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USplineMeshComponent* SplineMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

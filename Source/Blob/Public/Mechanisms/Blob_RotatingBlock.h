// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blob_Toggleable.h"
#include "GameFramework/Actor.h"
#include "Blob_RotatingBlock.generated.h"

UCLASS()
class BLOB_API ABlob_RotatingBlock : public AActor, public IBlob_Toggleable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlob_RotatingBlock();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_PlayerShadow.generated.h"

class ABlob_PlayerCharacter;

UCLASS()
class BLOB_API ABlob_PlayerShadow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlob_PlayerShadow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ShadowMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="Minimum distance between player and shadow"))
	float MinDistance = 50.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	ABlob_PlayerCharacter* PlayerCharacter;

	UFUNCTION(BlueprintCallable)
	void ToggleVisibility(bool bIsVisible);
};

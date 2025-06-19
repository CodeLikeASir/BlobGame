// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Blob_StaticMeshVariantComponent.generated.h"

UENUM(BlueprintType)
enum class EColorVariant : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	Blue UMETA(DisplayName = "Blue"),
	Green UMETA(DisplayName = "Green"),
	Red UMETA(DisplayName = "Red"),
	Yellow UMETA(DisplayName = "Yellow"),
};

/**
 * 
 */
UCLASS(ClassGroup = Rendering, meta = (BlueprintSpawnableComponent))
class BLOB_API UBlob_StaticMeshVariantComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UBlob_StaticMeshVariantComponent();
	
	virtual void OnComponentCreated() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=( DisplayPriority = 0 ), Category="Mesh")
	EColorVariant ColorVariant;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EColorVariant,UStaticMesh*> MeshVariants;

	UFUNCTION(CallInEditor, meta=( DisplayPriority = 0 ), Category="Mesh")
	void UpdateMesh();
};

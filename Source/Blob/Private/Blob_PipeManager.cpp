// Fill out your copyright notice in the Description page of Project Settings.


#include "Blob_PipeManager.h"

#include "Components/SplineMeshComponent.h"

// Sets default values
ABlob_PipeManager::ABlob_PipeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("SplineMesh"));
	RootComponent = SplineMesh;
}

// Called when the game starts or when spawned
void ABlob_PipeManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlob_PipeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


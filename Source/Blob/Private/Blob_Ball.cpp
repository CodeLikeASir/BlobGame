// Fill out your copyright notice in the Description page of Project Settings.


#include "Blob_Ball.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABlob_Ball::ABlob_Ball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	RootComponent = BallMesh;

	BallMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	BallMovement->SetActive(bUseProjectileMovement);
}

// Called when the game starts or when spawned
void ABlob_Ball::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlob_Ball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
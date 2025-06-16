// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Blob_PlayerShadow.h"
#include "Player/Blob_PlayerCharacter.h"

// Sets default values
ABlob_PlayerShadow::ABlob_PlayerShadow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShadowMesh = CreateDefaultSubobject<UStaticMeshComponent>("ShadowMesh");
	RootComponent = ShadowMesh;
}

// Called when the game starts or when spawned
void ABlob_PlayerShadow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlob_PlayerShadow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter == nullptr)
		return;

	FVector StartLocation = PlayerCharacter->GetActorLocation();
	FVector EndLocation = StartLocation - FVector(0.f, 0.f, 10000.f);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(PlayerCharacter);
	if (FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams))
	{
		ToggleVisibility(HitResult.Distance >= MinDistance);
		SetActorLocation(HitResult.Location);
	}
}

void ABlob_PlayerShadow::ToggleVisibility(bool bIsVisible)
{
	SetActorHiddenInGame(!bIsVisible);
}


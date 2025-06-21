// Fill out your copyright notice in the Description page of Project Settings.


#include "Blob_Checkpoint.h"

#include "Blob_CheckpointManager.h"
#include "Player/Blob_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABlob_Checkpoint::ABlob_Checkpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheckpointMesh"));
	RootComponent = CheckpointMesh;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABlob_Checkpoint::BeginPlay()
{
	Super::BeginPlay();

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ABlob_Checkpoint::OnOverlapBegin);

	if (CheckpointManager == nullptr)
	{
		if (AActor* Result = UGameplayStatics::GetActorOfClass(GetWorld(), ABlob_CheckpointManager::StaticClass());
			Result != nullptr)
		{
			CheckpointManager = Cast<ABlob_CheckpointManager>(Result);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("CheckpointManager not found"));
			Destroy();
		}
	} 
}

void ABlob_Checkpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bUnlocked)
	{
		return;
	}
	
	if (OtherActor->IsA(ABlob_PlayerCharacter::StaticClass()) &&
		CheckpointManager->CheckpointReached(CheckpointIndex))
	{
		bUnlocked = true;
		CheckpointMesh->SetStaticMesh(UnlockedMesh);

		OnCheckpointReached();
	}
}
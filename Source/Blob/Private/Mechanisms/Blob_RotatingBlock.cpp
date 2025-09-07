#include "Mechanisms/Blob_RotatingBlock.h"

ABlob_RotatingBlock::ABlob_RotatingBlock()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void ABlob_RotatingBlock::BeginPlay()
{
	Super::BeginPlay();
}

void ABlob_RotatingBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

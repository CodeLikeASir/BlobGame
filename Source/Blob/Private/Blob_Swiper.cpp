// (C) Jan Meissner


#include "Blob_Swiper.h"

// Sets default values
ABlob_Swiper::ABlob_Swiper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	RootComponent = PlatformMesh;

	SwiperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwiperMesh"));
	SwiperMesh->SetupAttachment(PlatformMesh);
}

// Called when the game starts or when spawned
void ABlob_Swiper::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlob_Swiper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsOn)
		return;
	
	float DeltaRotation = RotationSpeed * DeltaTime;
	FVector Rotation = RotationAxis * DeltaRotation;
	SwiperMesh->AddRelativeRotation(FRotator(Rotation.X, Rotation.Y, Rotation.Z));
}

void ABlob_Swiper::OnToggleOn_Implementation()
{
	IBlob_Toggleable::OnToggleOn_Implementation();
	bIsOn = true;
}

void ABlob_Swiper::OnToggleOff_Implementation()
{
	IBlob_Toggleable::OnToggleOff_Implementation();
	bIsOn = false;
}


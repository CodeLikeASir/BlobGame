// © Jan Meissner

#include "Mechanisms/Blob_Swiper.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
ABlob_Swiper::ABlob_Swiper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	RootComponent = PlatformMesh;

	SwiperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwiperMesh"));
	SwiperMesh->SetupAttachment(PlatformMesh);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotatingMovementComponent->RotationRate = FRotator(0.0f, 90.0f, 0.0f);
	RotatingMovementComponent->bRotationInLocalSpace = true;
	RotatingMovementComponent->SetUpdatedComponent(SwiperMesh);
}

// Called when the game starts or when spawned
void ABlob_Swiper::BeginPlay()
{
	Super::BeginPlay();

	RotationSpeed = RotatingMovementComponent->RotationRate;
}

// Called every frame
void ABlob_Swiper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlob_Swiper::OnToggleOn_Implementation()
{
	IBlob_Toggleable::OnToggleOn_Implementation();
	bIsOn = true;
	RotatingMovementComponent->RotationRate = RotationSpeed;
}

void ABlob_Swiper::OnToggleOff_Implementation()
{
	IBlob_Toggleable::OnToggleOff_Implementation();
	bIsOn = false;
	RotatingMovementComponent->RotationRate = FRotator::ZeroRotator;
}


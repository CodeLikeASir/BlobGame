


#include "Blob_Pickup.h"

#include "Player/Blob_PlayerCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ABlob_Pickup::ABlob_Pickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	PickupSphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABlob_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &ABlob_Pickup::OnOverlapBegin);
}

// Called every frame
void ABlob_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlob_Pickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ABlob_PlayerCharacter::StaticClass()))
	{
		ABlob_PlayerCharacter* Player = Cast<ABlob_PlayerCharacter>(OtherActor);
		Player->PickupCollectable();
		Destroy();
	}
}
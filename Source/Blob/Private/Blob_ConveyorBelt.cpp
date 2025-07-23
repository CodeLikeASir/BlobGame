


#include "Blob_ConveyorBelt.h"

#include "Components/BoxComponent.h"
#include "Player/Blob_PlayerCharacter.h"

// Sets default values
ABlob_ConveyorBelt::ABlob_ConveyorBelt()
{
	ConveyorBeltMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConveyorBeltMesh"));
	RootComponent = ConveyorBeltMesh;

	ConveyorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ConveyorTrigger"));
	ConveyorTrigger->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABlob_ConveyorBelt::BeginPlay()
{
	Super::BeginPlay();

	ConveyorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABlob_ConveyorBelt::OnOverlapBegin);
	ConveyorTrigger->OnComponentEndOverlap.AddDynamic(this, &ABlob_ConveyorBelt::OnOverlapEnd);
}

void ABlob_ConveyorBelt::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (ABlob_PlayerCharacter* Player = Cast<ABlob_PlayerCharacter>(OtherActor))
	{
		Player->GroundVelocity = -GetActorRightVector() * ConveyorSpeed;
	}
}

void ABlob_ConveyorBelt::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ABlob_PlayerCharacter* Player = Cast<ABlob_PlayerCharacter>(OtherActor))
	{
		Player->GroundVelocity = FVector::ZeroVector;
	}
}

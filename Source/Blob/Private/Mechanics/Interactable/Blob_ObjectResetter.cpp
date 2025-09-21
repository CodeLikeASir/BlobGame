#include "Mechanics/Interactable/Blob_ObjectResetter.h"

ABlob_ObjectResetter::ABlob_ObjectResetter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ValidArea = CreateDefaultSubobject<UBoxComponent>(TEXT("MoveableArea"));
	RootComponent = ValidArea;
	ValidArea->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void ABlob_ObjectResetter::BeginPlay()
{
	Super::BeginPlay();

	InitialTransforms = TArray<FTransform>();
	MonitoredObjects = TArray<AActor*>();
	ValidArea->OnComponentEndOverlap.AddDynamic(this, &ABlob_ObjectResetter::OnOverlapEnd);
}

void ABlob_ObjectResetter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("%s ended overlap with %s"), *GetName(), *OtherActor->GetName());

	if (!MonitoredObjects.Contains(OtherActor))
		return;

	if (MonitoredObjects.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No more objects to reset"));
		return;
	}

	if (MonitoredObjects.IndexOfByKey(OtherActor) >= InitialTransforms.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Looked up index for %s is out of bounds"), *OtherActor->GetName());
		return;
	}

	OtherActor->SetActorTransform(InitialTransforms[MonitoredObjects.IndexOfByKey(OtherActor)]);
	OtherActor->AddActorWorldOffset(FVector::UpVector * 100.0f);

	// Resets velocity of monitored actor if it uses physics
	if (UPrimitiveComponent* MovementComp = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()))
	{
		MovementComp->SetPhysicsLinearVelocity(FVector::ZeroVector, false);
	}
}

void ABlob_ObjectResetter::RegisterResettable(AActor* Resettable)
{
	InitialTransforms.Add(Resettable->GetActorTransform());
	MonitoredObjects.Add(Resettable);
}

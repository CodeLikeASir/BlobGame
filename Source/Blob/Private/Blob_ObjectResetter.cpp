// Fill out your copyright notice in the Description page of Project Settings.


#include "Blob_ObjectResetter.h"

// Sets default values
ABlob_ObjectResetter::ABlob_ObjectResetter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MoveableArea = CreateDefaultSubobject<UBoxComponent>(TEXT("MoveableArea"));
	RootComponent = MoveableArea;
	MoveableArea->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

// Called when the game starts or when spawned
void ABlob_ObjectResetter::BeginPlay()
{
	Super::BeginPlay();

	InitialTransforms = TArray<FTransform>();
	MonitoredObjects = TArray<AActor*>();
	MoveableArea->OnComponentEndOverlap.AddDynamic(this, &ABlob_ObjectResetter::OnOverlapEnd);
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
}

void ABlob_ObjectResetter::RegisterResettable(AActor* Resettable)
{
	InitialTransforms.Add(Resettable->GetActorTransform());
	MonitoredObjects.Add(Resettable);
}

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
	MoveableArea->OnComponentBeginOverlap.AddDynamic(this, &ABlob_ObjectResetter::OnOverlapStart);
	MoveableArea->OnComponentEndOverlap.AddDynamic(this, &ABlob_ObjectResetter::OnOverlapEnd);
}

// Called every frame
void ABlob_ObjectResetter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlob_ObjectResetter::OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("%s overlapped with %s"), *GetName(), *OtherActor->GetName());

	if (!OtherActor->ActorHasTag(ResettableTag) || MonitoredObjects.Contains(OtherActor))
		return;

	MonitoredObjects.Add(OtherActor);
	InitialTransforms.Add(OtherActor->GetActorTransform());
}

void ABlob_ObjectResetter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("%s ended overlap with %s"), *GetName(), *OtherActor->GetName());

	if (!OtherActor->ActorHasTag(ResettableTag) || !MonitoredObjects.Contains(OtherActor))
		return;

	OtherActor->SetActorTransform(InitialTransforms[MonitoredObjects.IndexOfByKey(OtherActor)]);
	OtherActor->AddActorWorldOffset(FVector::UpVector * 100.0f);
}

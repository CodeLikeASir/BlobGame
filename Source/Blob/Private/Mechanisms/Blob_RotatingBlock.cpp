#include "Mechanisms/Blob_RotatingBlock.h"

#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "Player/Blob_PlayerCharacter.h"

class ABlob_PlayerCharacter;

ABlob_RotatingBlock::ABlob_RotatingBlock()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	SpikeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SpikeCollision"));
	SpikeCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SpikeCollision->SetupAttachment(MeshComponent);
}

void ABlob_RotatingBlock::BeginPlay()
{
	Super::BeginPlay();
	
	SpikeCollision->OnComponentHit.AddDynamic(this, &ABlob_RotatingBlock::DamageActor);
}

void ABlob_RotatingBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlob_RotatingBlock::DamageActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
								FVector NormalImpulse, const FHitResult& Hit)
{
	if (ABlob_PlayerCharacter* Player = Cast<ABlob_PlayerCharacter>(OtherActor))
	{
		FDamageEvent DamageEvent;
		Player->TakeDamage(StunLength, DamageEvent, GetInstigatorController(), this);

		FVector HitDir = Hit.ImpactPoint - GetActorLocation();
		HitDir.Normalize();

		FVector PushForce = HitDir * StunForceMultiplier + StunUpForce;
		Player->AddStunForce(PushForce, StunLength);
	}
}
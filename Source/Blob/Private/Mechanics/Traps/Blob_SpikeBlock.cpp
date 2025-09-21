#include "Mechanics/Traps/Blob_SpikeBlock.h"

#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "Core/Blob_PlayerCharacter.h"

class ABlob_PlayerCharacter;

ABlob_SpikeBlock::ABlob_SpikeBlock()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	SpikeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SpikeCollision"));
	SpikeCollision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	SpikeCollision->SetNotifyRigidBodyCollision(true);
	SpikeCollision->SetupAttachment(MeshComponent);
}

void ABlob_SpikeBlock::BeginPlay()
{
	Super::BeginPlay();
	
	SpikeCollision->OnComponentHit.AddDynamic(this, &ABlob_SpikeBlock::DamageActor);
}

void ABlob_SpikeBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlob_SpikeBlock::DamageActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
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
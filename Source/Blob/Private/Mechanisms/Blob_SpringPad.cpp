


#include "Mechanisms/Blob_SpringPad.h"

#include "Player/Blob_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
ABlob_SpringPad::ABlob_SpringPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BaseIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseIndicator"));
	BaseIndicator->SetupAttachment(Root);

	SpringPad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpringPad"));
	SpringPad->SetupAttachment(Root);

	SpringConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("SpringConstraint"));
	SpringConstraint->ComponentName1 = FConstrainComponentPropName(FName("BaseIndicator"));
	SpringConstraint->ComponentName2 = FConstrainComponentPropName(FName("SpringPad"));
	SpringConstraint->SetDisableCollision(true);
	SpringConstraint->SetLinearZLimit(LCM_Limited, 70.0f);
}

// Called when the game starts or when spawned
void ABlob_SpringPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlob_SpringPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Velocity = SpringPad->GetComponentVelocity();
	if (Velocity.Z < -LaunchThreshold)
	{
		bCanLaunch = true;
	}
	else if (bCanLaunch && Velocity.Z > LaunchThreshold)
	{
		Launch();
		bCanLaunch = false;
	}
}

void ABlob_SpringPad::Launch()
{
	FVector UpVector = GetActorUpVector();
	FVector StartLocation = SpringPad->GetComponentLocation() + UpVector * 50.0f;
	FVector EndLocation = StartLocation + UpVector * 100.0f;

	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	if (UKismetSystemLibrary::SphereTraceMultiByProfile(
		GetWorld(), StartLocation, EndLocation, TraceRadius, FName("PhysicsActor"), false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration, OutHits, false, FLinearColor::Red, FLinearColor::Green, 1.0f))
	{
		for (auto OutHit : OutHits)
		{
			if (ABlob_PlayerCharacter* HitActor = Cast<ABlob_PlayerCharacter>(OutHit.GetActor()); HitActor != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, HitActor->GetName());
				FVector Velocity = HitActor->GetCapsuleComponent()->GetPhysicsLinearVelocity();
				HitActor->GetCapsuleComponent()->SetPhysicsLinearVelocity(Velocity + GetActorUpVector() * LaunchForce);
			}
		}
	}
}


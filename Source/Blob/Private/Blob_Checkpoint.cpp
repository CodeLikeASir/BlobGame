#include "Blob_Checkpoint.h"

#include "Systems/Blob_CheckpointManager.h"
#include "Player/Blob_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Blob_PlayerController.h"

// Sets default values
ABlob_Checkpoint::ABlob_Checkpoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheckpointMesh"));
	RootComponent = CheckpointMesh;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABlob_Checkpoint::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ABlob_Checkpoint::OnOverlapBegin);

	ABlob_CheckpointManager* Manager = Cast<ABlob_CheckpointManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ABlob_CheckpointManager::StaticClass()));
	if (CheckpointManager == nullptr)
	{
		CheckpointManager = Manager;
		CheckpointManager->RegisterCheckpoint(this);
	}
}

void ABlob_Checkpoint::BeginDestroy()
{
	Super::BeginDestroy();

	if (CheckpointManager)
	{
		CheckpointManager->UnregisterCheckpoint(this);
	}
}

void ABlob_Checkpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	if (bUnlocked)
	{
		return;
	}

	ABlob_PlayerCharacter* PlayerChar = Cast<ABlob_PlayerCharacter>(OtherActor);
	if (PlayerChar != nullptr)
	{
		OnCheckpointReached(PlayerChar);
	}
}

void ABlob_Checkpoint::SetUnlockedMesh()
{
	bUnlocked = true;
	CheckpointMesh->SetStaticMesh(UnlockedMesh);
}

void ABlob_Checkpoint::OnCheckpointReached(ABlob_PlayerCharacter* Player)
{
	ABlob_PlayerController* PlayerController = Cast<ABlob_PlayerController>(Player->Controller);
	EReachedResult Result = CheckpointManager->OnCheckpointReached(this, CheckpointIndex);

	if (Result == EReachedResult::GameEnd)
	{
		PlayerController->OnGameEndBP();
		return;
	}
	
	if (Result != EReachedResult::NewCheckpoint)
	{
		return;
	}

	SetUnlockedMesh();
	OnCheckpointReachedBP();
	PlayerController->OnCheckpointReached(CheckpointIndex);
}

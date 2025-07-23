

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_Ball.generated.h"

UCLASS()
class BLOB_API ABlob_Ball : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlob_Ball();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseProjectileMovement;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* BallMovement;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

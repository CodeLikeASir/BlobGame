#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_ConveyorBelt.generated.h"

UCLASS()
class BLOB_API ABlob_ConveyorBelt : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ConveyorBeltMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* ConveyorTrigger;

public:
	// Sets default values for this actor's properties
	ABlob_ConveyorBelt();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConveyorBelt")
	float ConveyorSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);
};

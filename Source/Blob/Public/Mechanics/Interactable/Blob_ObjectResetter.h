#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Blob_ObjectResetter.generated.h"

UCLASS()
class BLOB_API ABlob_ObjectResetter : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Object Resetter")
	UBoxComponent* MoveableArea;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Resetter")
	TArray<AActor*> MonitoredObjects;

	TArray<FTransform> InitialTransforms;

public:
	// Sets default values for this actor's properties
	ABlob_ObjectResetter();

protected: // Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void RegisterResettable(AActor* Resettable);
};

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Blob_ObjectResetter.generated.h"

/*
 * Monitors all actors that previously registered with it, resets their position to their initial value when exiting
 * the MoveableArea
 */
UCLASS()
class BLOB_API ABlob_ObjectResetter : public AActor
{
	GENERATED_BODY()

protected:
	/* Marks the bounds of where objects can be moved WITHOUT being reset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Object Resetter")
	UBoxComponent* ValidArea;

	/* Tracks all actors that should monitored and potentially reset */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Resetter")
	TArray<AActor*> MonitoredObjects;

	/* Saves the initial transforms (mainly location + rotation) of monitored actors on registration */
	TArray<FTransform> InitialTransforms;

public:
	ABlob_ObjectResetter();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);

	/* Registers a new actor to be monitored and reset on exit */
	UFUNCTION(BlueprintCallable)
	void RegisterResettable(AActor* Resettable);
};

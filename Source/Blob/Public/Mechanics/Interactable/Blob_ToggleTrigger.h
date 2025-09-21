#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_ToggleTrigger.generated.h"

/* Base class for an actor that can trigger functions on other actors on state change (on/off) */
UCLASS()
class BLOB_API ABlob_ToggleTrigger : public AActor
{
	GENERATED_BODY()

public:
	ABlob_ToggleTrigger();

protected:
	virtual void BeginPlay() override;

public:
	/* Actors on which the main interaction should be triggered */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Toggle")
	TArray<TSoftObjectPtr<AActor>> Toggleables;

	/* Actors on which a alternative interaction should be trigger */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Toggle")
	TArray<TSoftObjectPtr<AActor>> AltToggleables;

	/* Called when state is changed to on */
	UFUNCTION(BlueprintCallable)
	void OnToggleOn();

	/* Called when state is changed to off */
	UFUNCTION(BlueprintCallable)
	void OnToggleOff();
};



#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blob_Cannon.generated.h"

UCLASS()
class BLOB_API ABlob_Cannon : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnComponentMoved();
};

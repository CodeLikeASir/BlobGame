#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "Blob_InputRemapping.generated.h"

class UEnhancedInputUserSettings;
class UInputMappingContext;
class UInputAction;

/*
 * Allows for customization of keybindings
 */
UCLASS(BlueprintType)
class BLOB_API UBlob_InputRemapping : public UObject
{
	GENERATED_BODY()

public:
	UBlob_InputRemapping();

	UFUNCTION(BlueprintCallable)
	UEnhancedInputUserSettings* Init(UEnhancedInputLocalPlayerSubsystem* Subsystem, UInputMappingContext* BasicContext);

	UFUNCTION(BlueprintCallable)
	void RemapKey(FName ActionMappingName, FKey NewKey);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEnhancedInputUserSettings* UserSettings;
};

#include "Mechanics/Interactable/Blob_ToggleTrigger.h"

#include "Mechanics/Interactable/Blob_Toggleable.h"

// Sets default values
ABlob_ToggleTrigger::ABlob_ToggleTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABlob_ToggleTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void ABlob_ToggleTrigger::OnToggleOn()
{
	for (const TSoftObjectPtr<AActor>& ActorRef : Toggleables)
	{
		if (AActor* Actor = ActorRef.Get())
		{
			if (Actor->Implements<UBlob_Toggleable>())
			{
				IBlob_Toggleable::Execute_OnToggleOn(Actor);
			}
		}
	}

	for (const TSoftObjectPtr<AActor>& ActorRef : AltToggleables)
	{
		if (AActor* Actor = ActorRef.Get())
		{
			if (Actor->Implements<UBlob_Toggleable>())
			{
				IBlob_Toggleable::Execute_OnAltToggleOn(Actor);
			}
		}
	}
}

void ABlob_ToggleTrigger::OnToggleOff()
{
	for (const TSoftObjectPtr<AActor>& ActorRef : Toggleables)
	{
		if (AActor* Actor = ActorRef.Get())
		{
			if (Actor->Implements<UBlob_Toggleable>())
			{
				IBlob_Toggleable::Execute_OnToggleOff(Actor);
			}
		}
	}

	for (const TSoftObjectPtr<AActor>& ActorRef : AltToggleables)
	{
		if (AActor* Actor = ActorRef.Get())
		{
			if (Actor->Implements<UBlob_Toggleable>())
			{
				IBlob_Toggleable::Execute_OnAltToggleOff(Actor);
			}
		}
	}
}

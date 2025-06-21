// Fill out your copyright notice in the Description page of Project Settings.


#include "Blob_ToggleTrigger.h"

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
			if (IBlob_Toggleable* Interface = Cast<IBlob_Toggleable>(Actor))
			{
				Interface->OnToggleOn();
			}
		}
	}
	
	for (const TSoftObjectPtr<AActor>& ActorRef : AltToggleables)
	{
		if (AActor* Actor = ActorRef.Get())
		{
			if (IBlob_Toggleable* Interface = Cast<IBlob_Toggleable>(Actor))
			{
				Interface->OnAltToggleOn();
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
			if (IBlob_Toggleable* Interface = Cast<IBlob_Toggleable>(Actor))
			{
				Interface->OnToggleOff();
			}
		}
	}
	
	for (const TSoftObjectPtr<AActor>& ActorRef : AltToggleables)
	{
		if (AActor* Actor = ActorRef.Get())
		{
			if (IBlob_Toggleable* Interface = Cast<IBlob_Toggleable>(Actor))
			{
				Interface->OnAltToggleOff();
			}
		}
	}
}

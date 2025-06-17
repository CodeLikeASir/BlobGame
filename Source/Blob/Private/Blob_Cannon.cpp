// Fill out your copyright notice in the Description page of Project Settings.


#include "Blob_Cannon.h"

// In your Actor class implementation
#if WITH_EDITOR
void ABlob_Cannon::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
    
	if (bFinished)
	{
		OnComponentMoved(); // Call Blueprint event
	}
}

void ABlob_Cannon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	OnComponentMoved(); // Call Blueprint event
}
#endif
// Fill out your copyright notice in the Description page of Project Settings.


#include "Blob_StaticMeshVariantComponent.h"

UBlob_StaticMeshVariantComponent::UBlob_StaticMeshVariantComponent(): ColorVariant()
{
}

void UBlob_StaticMeshVariantComponent::OnComponentCreated()
{
	UpdateMesh();
}

void UBlob_StaticMeshVariantComponent::UpdateMesh()
{
	if (MeshVariants.IsEmpty())
		return;
	
	if (UStaticMesh* Mesh = MeshVariants[ColorVariant])
	{
		SetStaticMesh(Mesh);
	}
}

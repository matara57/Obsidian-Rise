// Fill out your copyright notice in the Description page of Project Settings.


#include "StructureManager.h"


UStructureManager::UStructureManager()
{
}
UStructureManager::UStructureManager(AStructureBase Constructable)
{

}

UStructureManager::~UStructureManager()
{
}


void UStructureManager::ServerPlaceStructure_Implementation(TSubclassOf<AStructureBase> Structure, FTransform Transform)
{
	FActorSpawnParameters SpawnInfo;
	UE_LOG(LogTemp, Warning, TEXT("Hello"));
	AStructureBase* SpawnedStructure = GetWorld()->SpawnActor<AStructureBase>(Structure, Transform, SpawnInfo);
	SpawnedStructure->bShouldUpdatePreviewTransform = false;
}

bool UStructureManager::ServerPlaceStructure_Validate(TSubclassOf<AStructureBase> Structure, FTransform Transform)
{
	return true;
}
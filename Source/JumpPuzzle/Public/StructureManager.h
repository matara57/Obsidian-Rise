// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <StructureBase.h>
#include <Net/UnrealNetwork.h>
#include "Engine/NetSerialization.h"
#include "StructureManager.generated.h"



/**
 * 
 */
UCLASS(BlueprintType)
class JUMPPUZZLE_API UStructureManager : public UObject
{
public:
	GENERATED_BODY()

	UStructureManager();
	UStructureManager(AStructureBase Constructable);
	~UStructureManager();


	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerPlaceStructure(TSubclassOf<AStructureBase> Structure, FTransform Transform);
	void ServerPlaceStructure_Implementation(TSubclassOf<AStructureBase> Structure, FTransform Transform);
	bool ServerPlaceStructure_Validate(TSubclassOf<AStructureBase> Structure, FTransform Transform);

};

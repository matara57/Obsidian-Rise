// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include <StructureManager.h>
#include "StandardGameState.generated.h"


/**
 * 
 */
UCLASS()
class JUMPPUZZLE_API AStandardGameState : public AGameState
{
	GENERATED_BODY()

	AStandardGameState();


	void HasMatchStarted();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStructureManager* StructureManager;
	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "StandardGameState.h"

AStandardGameState::AStandardGameState() {
	

}

void AStandardGameState::HasMatchStarted()
{
	StructureManager = CreateDefaultSubobject<UStructureManager>(TEXT("StructureManager"));
}

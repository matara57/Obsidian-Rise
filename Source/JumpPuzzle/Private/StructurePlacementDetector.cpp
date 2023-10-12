// Fill out your copyright notice in the Description page of Project Settings.


#include "StructurePlacementDetector.h"
#include <Net/UnrealNetwork.h>

// Sets default values for this component's properties
AStructurePlacementDetector::AStructurePlacementDetector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;
	bIsColliding = false;
	bIsOnTerrain = false;
	
}


// Called when the game starts
void AStructurePlacementDetector::BeginPlay()
{
	Super::BeginPlay();
	// ...
	UE_LOG(LogTemp, Log, TEXT("Registered"));
}


// Called every frame
void AStructurePlacementDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ROLE_Authority && BoxCollider != nullptr) {
		FVector drawPos = BoxCollider->GetComponentToWorld().GetLocation();
		FColor drawColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f).ToFColor(true);
		float drawDuration = 0.0f;
		bool drawShadow = true;
		if (bIsColliding || !bIsOnTerrain) {
			DrawDebugString(GetWorld(), drawPos, *FString::Printf(TEXT(" %s \n  %s"), bIsColliding ? TEXT("IsColliding: true") : TEXT(""), bIsOnTerrain ? TEXT("") : TEXT("IsOnTerrain: false")), NULL, drawColor, drawDuration, drawShadow);
		}
	}



}
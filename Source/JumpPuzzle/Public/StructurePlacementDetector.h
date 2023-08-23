// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(disable: 4800)

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Components/BoxComponent.h>

#include "StructurePlacementDetector.generated.h"


UCLASS(Blueprintable)
class JUMPPUZZLE_API AStructurePlacementDetector : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AStructurePlacementDetector();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* BoxCollider = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bIsColliding;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bIsOnTerrain;
		
};

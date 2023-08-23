// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructureBase.h"
#include "SocketableStructure.generated.h"

/**
 * 
 */
UCLASS()
class JUMPPUZZLE_API ASocketableStructure : public AStructureBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ASocketableStructure();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsSocketed;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketableStructure.h"

ASocketableStructure::ASocketableStructure()
{
	bIsSocketed = false;
}

void ASocketableStructure::BeginPlay()
{
	Super::BeginPlay();

}

void ASocketableStructure::PivotTo(EPivotPoint ToPivotPoint)
{
	FVector BoundsMin;
	FVector BoundsMax;
	MeshComponent->GetLocalBounds(BoundsMin, BoundsMax);
	ActivePivotPoint = ToPivotPoint;
	switch (ToPivotPoint)
	{
	case Start:
		MeshComponent->SetRelativeLocation(FVector(BoundsMax.X, 0, 0));
		break;
	case Center:
		MeshComponent->SetRelativeLocation(FVector(0, 0, 0));
		break;
	case End:
		MeshComponent->SetRelativeLocation(FVector(BoundsMin.X, 0, 0));
		break;
	default:
		break;
	}
}
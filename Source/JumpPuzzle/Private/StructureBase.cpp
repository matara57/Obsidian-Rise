// Fill out your copyright notice in the Description page of Project Settings.

#include "StructureBase.h"
#include <StructurePlacementDetector.h>


// Sets default values
AStructureBase::AStructureBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	IsColliding = false;
	IsOnFlatSurface = false;
	bIsSafeToPlace = false;
	static ConstructorHelpers::FObjectFinder<UMaterial> TValidMaterial(TEXT("Material'/Game/Materials/M_BuildingPreview.M_BuildingPreview'"));
	if (TValidMaterial.Object != NULL)
	{
		ValidMaterial = (UMaterial*)TValidMaterial.Object;

	}

	static ConstructorHelpers::FObjectFinder<UMaterial> TInvalidMaterial(TEXT("Material'/Game/Materials/M_BuildingPreviewInvalid.M_BuildingPreviewInvalid'"));
	if (TInvalidMaterial.Object != NULL)
	{
		InvalidMaterial = (UMaterial*)TInvalidMaterial.Object;
	
	}

	PivotPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PivotPoint"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrimaryMesh"));


	MeshComponent->SetupAttachment(PivotPoint);
	AddOwnedComponent(PivotPoint);
	PivotPoint->SetIsReplicated(true);
	MeshComponent->SetIsReplicated(true); // Enable replication by default
}

// Called when the game starts or when spawned
void AStructureBase::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->AttachToComponent(PivotPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	if (InvalidMaterial != nullptr && ValidMaterial != nullptr) {
		ValidPlacementMaterial = UMaterialInstanceDynamic::Create(this->ValidMaterial, this);
		InvalidPlacementMaterial = UMaterialInstanceDynamic::Create(this->InvalidMaterial, this);
	}

	//We call this at start because if the bool is false, then we won't show the clients the InvalidPlacement material
	OnRep_IsSafeToPlace();
	OnRep_IsPreview();
}

// Called every frame
void AStructureBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStructureBase::ServerPlacementSafetyChanged_Implementation() {

	//This is called in a server RPC because RepNotify doesn't run on the server when a variable changes. 
	OnRep_IsSafeToPlace();
}

void AStructureBase::PlacementSafetyChanged()
{
	OnRep_IsSafeToPlace();
	ServerPlacementSafetyChanged();
}

bool AStructureBase::ServerPlacementSafetyChanged_Validate()
{
	return true;
}

void AStructureBase::ServerChangeMaterial_Implementation(UMaterial* Material)
{

}

void AStructureBase::OnRep_IsSafeToPlace()
{
	if (R_bIsPreview) {
		UMaterialInstanceDynamic* R_ActiveMaterial;
		if (bIsSafeToPlace) {
			R_ActiveMaterial = ValidPlacementMaterial;
		}
		else {
			R_ActiveMaterial = InvalidPlacementMaterial;
		}

		MeshComponent->SetMaterial(0, R_ActiveMaterial);
	}

}

void AStructureBase::OnRep_IsPreview()
{
	if (R_bIsPreview) {

		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AStructureBase::ServerUpdatePreviewTransform_Implementation(FVector_NetQuantize Transform)
{
	CurrentTransform = Transform;
	OnRep_CurrentTransform(Transform);
}

bool AStructureBase::ServerUpdatePreviewTransform_Validate(FVector_NetQuantize Transform)
{
	return true;
}

void AStructureBase::UpdatePreviewTransform(FVector_NetQuantize Transform)
{
	SetActorLocation(FVector((static_cast<int>(Transform.X) / 50 * 50), (static_cast<int>(Transform.Y) / 50 * 50), Transform.Z));
}

void AStructureBase::ServerUpdatePreviewRotation_Implementation(FVector_NetQuantize Transform)
{

	AddActorWorldRotation(FRotator(Transform.X, Transform.Y, Transform.Z));
}

bool AStructureBase::ServerUpdatePreviewRotation_Validate(FVector_NetQuantize Transform)
{
	return true;
}

void AStructureBase::OnRep_CurrentTransform(FVector_NetQuantize Transform)
{
	SetActorTransform(FTransform(Transform.Rotation(), FVector(Transform.X, Transform.Y, Transform.Z)));
	//SetActorLocation(FVector((static_cast<int>(Transform.X) / 50 * 50), (static_cast<int>(Transform.Y) / 50 * 50), Transform.Z));

}

void AStructureBase::RegisterPlacementDetector(AStructurePlacementDetector* PlacementDetector)
{
	PlacementDetectors.Add(PlacementDetector);
}

//This is required whenever any class wants to RepNotify variables.
void AStructureBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Macro used for every variable that we want to RepNotify on changes
	DOREPLIFETIME(AStructureBase, R_bIsPreview);
	DOREPLIFETIME_CONDITION(AStructureBase, CurrentTransform, COND_SkipOwner);

}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
#include "Net/UnrealNetwork.h"
#include <StructurePlacementDetector.h>
#include "StructureBase.generated.h"

UENUM(BlueprintType)
enum ESnappingRule {
	None,
	Floor,
	Wall,
	Roof
};

UENUM(BlueprintType)
enum EPivotPoint : uint8
{
	Start,
	Center,
	End
};

USTRUCT(BlueprintType)
struct FStructureCost : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere);
	FDataTableRowHandle Resource;

	UPROPERTY(EditAnywhere);
	int32 Quantity;

};

USTRUCT(BlueprintType)
struct FStructureValues : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName GUID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TArray<FStructureCost> Costs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TObjectPtr<UTexture2D> MenuIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsAvailableToPlayer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftClassPtr<AStructureBase> Actor;


};


USTRUCT()
struct FStructureDataRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FText DisplayName;

	UPROPERTY(EditAnywhere)
		float FlowerRadius = 0.5f;

	UPROPERTY(EditAnywhere)
		TArray<FDataTableRowHandle> ChildPlants;
};

UCLASS()
class JUMPPUZZLE_API AStructureBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStructureBase();

	void SetupStructure();

	//Replication
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_IsSafeToPlace();

	UFUNCTION(BlueprintCallable)
	virtual void OnRep_IsPreview();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerPlacementSafetyChanged();
	void ServerPlacementSafetyChanged_Implementation();
	bool ServerPlacementSafetyChanged_Validate();

	UFUNCTION(BlueprintCallable)
	void PlacementSafetyChanged();

	UFUNCTION(Server, Reliable)
	void ServerChangeMaterial(UMaterial* Material);

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void ServerUpdatePreviewTransform(FVector_NetQuantize Transform);
	void ServerUpdatePreviewTransform_Implementation(FVector_NetQuantize Transform);
	bool ServerUpdatePreviewTransform_Validate(FVector_NetQuantize Transform);

	UFUNCTION(BlueprintCallable)
	void UpdatePreviewTransform(FVector_NetQuantize Transform);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerUpdatePreviewRotation(FVector_NetQuantize Transform);
	void ServerUpdatePreviewRotation_Implementation(FVector_NetQuantize Transform);
	bool ServerUpdatePreviewRotation_Validate(FVector_NetQuantize Transform);

	UFUNCTION(BlueprintCallable)
	void OnRep_CurrentTransform(FVector_NetQuantize Transform);

	UFUNCTION(BlueprintCallable)
	void RegisterPlacementDetector(AStructurePlacementDetector* PlacementDetector);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere);
	bool bIsSafeToPlace;

	UPROPERTY(ReplicatedUsing = OnRep_IsPreview, BlueprintReadWrite);
	bool R_bIsPreview;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDataTableRowHandle RowHandle;

	UPROPERTY(BlueprintReadWrite);
	bool IsColliding;

	UPROPERTY(BlueprintReadWrite);
	bool IsOnFlatSurface;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USceneComponent* PivotPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AStructurePlacementDetector* > PlacementDetectors;

	UPROPERTY();
	UMaterialInstanceDynamic* ValidPlacementMaterial;

	UPROPERTY();
	UMaterialInstanceDynamic* InvalidPlacementMaterial;

	UPROPERTY();
	UMaterial* ValidMaterial;

	UPROPERTY();
	UMaterial* InvalidMaterial;

	UPROPERTY()
	FTimerHandle UnusedHandle;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentTransform)
	FVector_NetQuantize CurrentTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bShouldUpdatePreviewTransform = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EPivotPoint> ActivePivotPoint = EPivotPoint::Center;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UShapeComponent* AttachedTo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ESnappingRule> SnappingRule;
};
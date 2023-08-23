// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include <StructureBase.h>
#include "InputMappingContext.h"
#include <StructureManager.h>
#include "BuildSystemComponent.generated.h"



UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JUMPPUZZLE_API UBuildSystemComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* BuildModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* PlaceBuildingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SecondaryRotateAction;
	

public:	
	// Sets default values for this component's properties
	UBuildSystemComponent();

	void AddStructuresFromTable(const FName& RowName, const FStructureValues& DataRow);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	UFUNCTION()
	void PreviewLoop();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerShowStructurePreview(TSubclassOf<AStructureBase> Structure);
	void ServerShowStructurePreview_Implementation(TSubclassOf<AStructureBase> Structure);
	bool ServerShowStructurePreview_Validate(TSubclassOf<AStructureBase> Structure);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPlaceStructure(TSubclassOf<AStructureBase> Structure, FTransform Transform);
	void ServerPlaceStructure_Implementation(TSubclassOf<AStructureBase> Structure, FTransform Transform);
	bool ServerPlaceStructure_Validate(TSubclassOf<AStructureBase> Structure, FTransform Transform);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetRotation(FRotator Rotation);
	void ServerSetRotation_Implementation(FRotator Rotation);
	bool ServerSetRotation_Validate(FRotator Rotation);

	UFUNCTION(BlueprintCallable)
	void ToggleBuildMode(bool OverrideState, bool NewStateValue);

	UFUNCTION()
	void AddRotation(const FInputActionValue& Value);

	UFUNCTION()
	void PlaceStructure();

	UFUNCTION(BlueprintCallable)
	void SetSelectedStructure(TSubclassOf<AStructureBase> Structure);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStructureManager* StructureManager;

	UPROPERTY(EditAnywhere, Category = "Input")
		TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category = "Collision")
		TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_WorldStatic;

	UPROPERTY()
		UCameraComponent* PlayerCamera;

	UPROPERTY(BlueprintReadOnly)
		bool bBuildModeActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AStructureBase> SelectedStructure;

	UPROPERTY(Replicated, BlueprintReadOnly)
		AStructureBase* StructurePreview;

	UPROPERTY(BlueprintReadOnly)
		FTransform PreviewTransform;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray < FStructureValues> Structures;

	UPROPERTY(BlueprintReadOnly)
		FHitResult PlayerViewHit;

};

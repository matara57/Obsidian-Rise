// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildSystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "StructureManager.h"
#include "Kismet/GameplayStatics.h"
#include <StandardGameState.h>
#include "StandardGameMode.h"
#include "StandardCharacter.h"
#include "Engine/World.h"
#include <SocketableStructure.h>

UBuildSystemComponent::UBuildSystemComponent()
{
	SetIsReplicated(true);
	PrimaryComponentTick.bCanEverTick = true;

	UDataTable* StructuresDataTable = nullptr;

	static ConstructorHelpers::FObjectFinder<UDataTable> FoundStructuresTable(TEXT("DataTable'/Game/DataTables/DT_Structures_Buildable.DT_Structures_Buildable'"));
	if (FoundStructuresTable.Object != NULL)
	{
		StructuresDataTable = (UDataTable*)FoundStructuresTable.Object;
	}

	TArray < FStructureValues* > localStructures;
	if (StructuresDataTable != nullptr)
	{
		StructuresDataTable->GetAllRows<FStructureValues>(TEXT("OH NO"), localStructures);

		for (FStructureValues* ptr : localStructures)
		{
			if (ptr)
			{
				Structures.Push(*ptr);
			}
		}
	}
}

// Called when the game starts
void UBuildSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	AStandardCharacter* PlayerCharacter = Cast<AStandardCharacter>(GetOwner());

	if (PlayerCharacter != nullptr) {
		PlayerCamera = PlayerCharacter->GetFollowCamera();
	}

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetWorld()->GetFirstLocalPlayerFromController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 1);
			}
		}
	}

	AStandardGameState* StandardGameState = Cast< AStandardGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (StandardGameState != nullptr) {
		StructureManager = StandardGameState->StructureManager;
	}
}


// Called every frame
void UBuildSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwnerRole() == ROLE_AutonomousProxy || GetOwnerRole() == ROLE_Authority) {
		PreviewLoop();
	}

}

void UBuildSystemComponent::PreviewLoop() {

	PlayerViewHit;
	FVector LineStart = PlayerCamera->GetComponentLocation();
	FVector LineEnd = PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * 2200.0f;

	FVector TraceStart = LineStart;
	FVector TraceEnd = LineEnd;

	const FName TraceTag("PlayerLookTag");

	GetWorld()->DebugDrawTraceTag = TraceTag;

	FCollisionQueryParams QueryParams;
	QueryParams.TraceTag = TraceTag;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(StructurePreview);

	GetWorld()->LineTraceSingleByChannel(PlayerViewHit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

	if (PlayerViewHit.bBlockingHit && IsValid(PlayerViewHit.GetActor()))
	{
		if (StructureManager == nullptr || StructurePreview == nullptr || !StructurePreview->bShouldUpdatePreviewTransform) return;
		StructurePreview->OnRep_CurrentTransform(PlayerViewHit.Location);
		StructurePreview->CurrentTransform = PlayerViewHit.Location;
		/*StructurePreview->UpdatePreviewTransform(FVector_NetQuantize(PlayerViewHit.Location));
		StructurePreview->ServerUpdatePreviewTransform(PlayerViewHit.Location);*/
		StructurePreview->SetActorRotation(FQuat(0.0f, 0.0f, 0.0f, 0.0f));
	}
}

#pragma region ServerRPCs

void UBuildSystemComponent::ServerShowStructurePreview_Implementation(TSubclassOf<AStructureBase> Structure)
{
	if (StructurePreview != nullptr) {
		StructurePreview->Destroy();
	}
	FActorSpawnParameters SpawnInfo;
	StructurePreview = GetWorld()->SpawnActorDeferred<AStructureBase>(Structure, PreviewTransform, GetOwner());


	if (StructurePreview) {
		StructurePreview->R_bIsPreview = true;
		StructurePreview->SetAutonomousProxy(true);
		UGameplayStatics::FinishSpawningActor(StructurePreview, StructurePreview->GetActorTransform());

	}
}

bool UBuildSystemComponent::ServerShowStructurePreview_Validate(TSubclassOf<AStructureBase> Structure)
{
	return true;
}

void UBuildSystemComponent::ServerPlaceStructure_Implementation(TSubclassOf<AStructureBase> Structure, FTransform Transform)
{
	AStandardGameState* StandardGameState = Cast< AStandardGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (StandardGameState != nullptr) {
		StructureManager = StandardGameState->StructureManager;
	}
	if (StructureManager == nullptr || Structure == nullptr || StructurePreview == nullptr || !StructurePreview->bIsSafeToPlace) return;

	StructureManager->ServerPlaceStructure(Structure, Transform);
}

bool UBuildSystemComponent::ServerPlaceStructure_Validate(TSubclassOf<AStructureBase> Structure, FTransform Transform)
{
	return true;
}

void UBuildSystemComponent::ServerSetRotation_Implementation(FRotator Rotation)
{
	if (StructurePreview == nullptr) return;

	StructurePreview->SetActorRotation(Rotation);
}

bool UBuildSystemComponent::ServerSetRotation_Validate(FRotator Rotation)
{
	return true;
}

#pragma endregion

void UBuildSystemComponent::ToggleBuildMode(bool OverrideState, bool NewStateValue)
{
	bBuildModeActive = !bBuildModeActive;
}

void UBuildSystemComponent::PlaceStructure()
{
	if (SelectedStructure == nullptr || StructurePreview == nullptr) return;
	ASocketableStructure* SocketableStructure = Cast<ASocketableStructure>(StructurePreview);
	if (SocketableStructure == nullptr) {
		ServerPlaceStructure(SelectedStructure, StructurePreview->GetActorTransform());
	}
	else {
		FHitResult adfg;
		StructurePreview->K2_AddActorLocalOffset(StructurePreview->MeshComponent->GetRelativeLocation(), false, adfg, true);
		
		ServerPlaceStructure(SelectedStructure, StructurePreview->GetActorTransform());
	}


	if (SocketableStructure == nullptr) return;
	UClass* existingStructure = StructurePreview->GetClass();
	ServerShowStructurePreview(StructurePreview->GetClass());
}


void UBuildSystemComponent::AddRotation(const FInputActionValue& Value)
{
	if (StructurePreview == nullptr) return;

	FVector Rotation = FVector(0, 30 * Value.GetMagnitude(), 0);
	StructurePreview->AddActorWorldRotation(FRotator(Rotation.X, Rotation.Y, Rotation.Z));
	ServerSetRotation(StructurePreview->GetActorRotation());
}

void UBuildSystemComponent::SetSelectedStructure(TSubclassOf<AStructureBase> Structure)
{
	SelectedStructure = Structure;
}

#pragma region Initialization

void UBuildSystemComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Macro used for every variable that we want to RepNotify on changes
	DOREPLIFETIME(UBuildSystemComponent, StructurePreview);
}

void UBuildSystemComponent::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(PlaceBuildingAction, ETriggerEvent::Triggered, this, &UBuildSystemComponent::PlaceStructure);
		EnhancedInputComponent->BindAction(BuildModeAction, ETriggerEvent::Triggered, this, FName("UBuildSystemComponent::ToggleBuildMode"));
		EnhancedInputComponent->BindAction(SecondaryRotateAction, ETriggerEvent::Triggered, this, &UBuildSystemComponent::AddRotation);
	}
}

#pragma endregion
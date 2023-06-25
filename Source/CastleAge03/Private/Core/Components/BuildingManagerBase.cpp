// Code written by Zachary Parker for Feldfeste 


#include "Core/Components/BuildingManagerBase.h"
#include "Core/Actors/BuildingMasterBase.h"
#include "Core/Player/RTSPlayerControllerBase.h"
#include "Core/Library/GameTimeFunc.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UBuildingManagerBase::UBuildingManagerBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Replicated variables
void UBuildingManagerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBuildingManagerBase, BuildingTransform);
	DOREPLIFETIME(UBuildingManagerBase, BuildingRowName);
	DOREPLIFETIME(UBuildingManagerBase, ListOfUnderConstructionBuildings);
	DOREPLIFETIME(UBuildingManagerBase, ListOfBuildings);
	DOREPLIFETIME(UBuildingManagerBase, GameSpeed);
	DOREPLIFETIME(UBuildingManagerBase, SpeedMutlipler);
}

// Called when the game starts
void UBuildingManagerBase::BeginPlay()
{
	Super::BeginPlay();

	// ControllerRef
	ControllerRef = Cast<APlayerController>(GetOwner());
	GetWorld()->GetGameState<ARTSGameState>()->GameSpeedControl.AddDynamic(this, &UBuildingManagerBase::UpdateGameSpeed);
	GameSpeed = GetWorld()->GetGameState<ARTSGameState>()->GetGameTime();
	UpdateGameSpeed(GameSpeed);
}

// Called every frame
void UBuildingManagerBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/************************************************
* GLOBAL BUILDING RELATED FUNCTIONS
************************************************/
void UBuildingManagerBase::UpdateGameSpeed(float inGameSpeed)
{
	GameSpeed = inGameSpeed;
	int SpeedSettingLocal = UGameTimeFunc::GetGameSpeedInt(GameSpeed);
	SpeedMutlipler = UGameTimeFunc::SetSpeedMultiplier(SpeedSettingLocal);
#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("Manager Speed Multiplier is %f"), SpeedMutlipler);
#endif
}

/************************************************
* PREVIEW/GHOST BUILDING METHODS
************************************************/

void UBuildingManagerBase::SetIsPreviewing(bool bValue)
{
	bIsPreviewing = bValue;
}

void UBuildingManagerBase::SetMousePosition(FVector ControllerMousePosition)
{
	MousePosition = ControllerMousePosition;
}

void UBuildingManagerBase::ROC_SetPreview_Implementation(FName RowName)
{
	// check if the player is already in preview
	if (GetIsPreviewing())
	{
		// if they are Destroy the current preview
		ROC_DestroyPreview();
	}
	//proceed with making a new preview

	//Spawn Actor
	ROC_SpawnPreviewBuilding(RowName);

	//Set is previewing
	SetIsPreviewing(true);

	if (BuildingRef->GetClass()->ImplementsInterface(UBuildingInterface::StaticClass()))
	{
		IBuildingInterface::Execute_TriggerSetPreviewMaterial(BuildingRef);
	}

	// begin timer for tracking the mouse/moving the preview
	GetWorld()->GetTimerManager().SetTimer(PreviewTimerHandle, this, &UBuildingManagerBase::ROC_TrackMouse_Implementation, 0.01f, true);
}

void UBuildingManagerBase::ROC_SpawnPreviewBuilding_Implementation(FName RowName)
{
	// local varaibles for the spawning
	FTransform TransformLocal(GetGridSnaps());
	TSubclassOf<ABuildingMasterBase> BuildingClassLocal;

	// use deferred spawn to begin all the various aspects the system needs
	ABuildingMasterBase* PreviewBuildingLocal = Cast<ABuildingMasterBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ABuildingMasterBase::StaticClass(), TransformLocal));
	if (PreviewBuildingLocal != nullptr)
	{
		BuildingRef = PreviewBuildingLocal;

		// Set the building name, preview status and owner
		if (BuildingRef->GetClass()->ImplementsInterface(UBuildingInterface::StaticClass()))
		{
			IBuildingInterface::Execute_TriggerSetRowName(BuildingRef, RowName);
			IBuildingInterface::Execute_TriggerSetOwner(BuildingRef, ControllerRef);
			IBuildingInterface::Execute_TriggerSetConstructionStatus(BuildingRef, EConstructionStatus::Preview);
		}
		// finish spawning the building.
		UGameplayStatics::FinishSpawningActor(BuildingRef, TransformLocal);
	}
}

void UBuildingManagerBase::ROC_TrackMouse_Implementation()
{
	if (GetPreviewBuilding() != nullptr)
	{
		if (GetPreviewBuilding()->GetClass()->ImplementsInterface(UBuildingInterface::StaticClass()))
		{
			IBuildingInterface::Execute_TriggerMovePreview(GetPreviewBuilding(), GetGridSnaps());
		}
	}
}

void UBuildingManagerBase::RotatePreviewLeft()
{
	//rotators are y (pitch), z (yaw), x (roll).
	if (GetPreviewBuilding() != nullptr)
	{
		if (GetPreviewBuilding()->GetClass()->ImplementsInterface(UBuildingInterface::StaticClass()))
		{
			IBuildingInterface::Execute_TriggerRotatePreview(GetPreviewBuilding(), FRotator(GetPreviewBuilding()->GetActorRotation().Pitch, GetPreviewBuilding()->GetActorRotation().Yaw + 15.0f, GetPreviewBuilding()->GetActorRotation().Roll));
		}
	}
}

void UBuildingManagerBase::RotatePreviewRight()
{
	if (GetPreviewBuilding() != nullptr)
	{
		if (GetPreviewBuilding()->GetClass()->ImplementsInterface(UBuildingInterface::StaticClass()))
		{
			IBuildingInterface::Execute_TriggerRotatePreview(GetPreviewBuilding(), FRotator(GetPreviewBuilding()->GetActorRotation().Pitch, GetPreviewBuilding()->GetActorRotation().Yaw - 15.0f, GetPreviewBuilding()->GetActorRotation().Roll));
		}
	}
}

void UBuildingManagerBase::ROC_DestroyPreview_Implementation()
{
	// check if the building preview is valid, not a null pointer.
	if (GetPreviewBuilding())
	{
		// tell preview to trigger it's destroypreview function.
		if (GetPreviewBuilding()->GetClass()->ImplementsInterface(UBuildingInterface::StaticClass()))
		{
			IBuildingInterface::Execute_TriggerDestroyPreview(GetPreviewBuilding());
		}
		// clear reference
		BuildingRef = NULL;

		// turn off previewing mode and clear timer
		SetIsPreviewing(false);
		GetWorld()->GetTimerManager().ClearTimer(PreviewTimerHandle);
	}
}

/************************************************
* CONSTRUCTION METHODS
************************************************/

void UBuildingManagerBase::ROC_StoreVariables_Implementation(FTransform TargetTransform, FName InRowName)
{
	BuildingTransform = TargetTransform;
	BuildingRowName = InRowName;
	//BuildingTransform = GetPreviewBuilding()->GetActorTransform();
	//BuildingRowName = GetPreviewBuilding()->GetRowName();
}

void UBuildingManagerBase::ROC_RunConstructionStart_Implementation(FTransform TargetTransform, FName InRowName) // DID NOT WORK. REMOVE
{
	/*ROS_SpawnConstruction_Implementation(TargetTransform, InRowName);
	ROC_CallSetSatus(BuildingRef);
	ROC_AddToUnconstructedList(BuildingRef);
	// start construction check
	ROC_SetConstructionTimer();*/
	IBuildingInterface::Execute_TriggerSetOwner(BuildingRef, ControllerRef);
}

void UBuildingManagerBase::ROS_SpawnConstruction_Implementation(FTransform TargetTransform, FName InRowName)
{
	TSubclassOf<ABuildingMasterBase> BuildingClassLocal;

	// use deferred spawn to begin all the various aspects the system needs
	ABuildingMasterBase* ConstructionBuildingLocal = Cast<ABuildingMasterBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ABuildingMasterBase::StaticClass(), TargetTransform));
	if (ConstructionBuildingLocal != nullptr)
	{
		BuildingRef = ConstructionBuildingLocal;

		if (BuildingRef->GetClass()->ImplementsInterface(UBuildingInterface::StaticClass()))
		{
			IBuildingInterface::Execute_TriggerSetRowName(BuildingRef, InRowName);
			IBuildingInterface::Execute_TriggerSetOwner(BuildingRef, ControllerRef);
			IBuildingInterface::Execute_TriggerSetConstructionStatus(BuildingRef, EConstructionStatus::UnderConstruction);
			IBuildingInterface::Execute_TriggerSetBuildingStatus(BuildingRef, 0);


			// finish spawning the building.
			UGameplayStatics::FinishSpawningActor(BuildingRef, TargetTransform);
			ROC_SetBuildingRef(BuildingRef);
			ROC_CallSetSatus(BuildingRef);
			ROC_AddToUnconstructedList(BuildingRef);
			// start construction check
			ROC_SetConstructionTimer();
		}
		else
		{
			BuildingRef->Destroy();
		}
	}
}

void UBuildingManagerBase::ROC_SetBuildingRef_Implementation(ABuildingMasterBase* TargetBuilding)
{
	if (!ensure(TargetBuilding != nullptr)) return;
	BuildingRef = TargetBuilding;
}

void UBuildingManagerBase::ROC_CallSetSatus_Implementation(ABuildingMasterBase* NewBuilding)
{
	if (!ensure(NewBuilding != nullptr)) return;
	IBuildingInterface::Execute_TriggerSetBuildingStatus(NewBuilding, 0);
}

void UBuildingManagerBase::ROC_AddToUnconstructedList_Implementation(ABuildingMasterBase* NewBuilding)
{
	if (!ensure(NewBuilding != nullptr)) return;
	ListOfUnderConstructionBuildings.AddUnique(NewBuilding);
}

void UBuildingManagerBase::ROC_RemoveToUnconstructedList_Implementation(ABuildingMasterBase* OldBuilding)
{
	if (!ensure(OldBuilding != nullptr)) return;

	if (ListOfUnderConstructionBuildings.Num() >= 1)
	{
		ListOfUnderConstructionBuildings.RemoveSingle(OldBuilding);
	}
}

void UBuildingManagerBase::ROC_SetConstructionTimer_Implementation()
{
	// confirm there is one valid index.
	if (ListOfUnderConstructionBuildings.Last() >= 0)
	{
		// check if the timer handle exists
		if (!ConstructionTimerHandle.IsValid())
		{
			// if it doesn't, create timer that fires ever .1s.
			GetWorld()->GetTimerManager().SetTimer(ConstructionTimerHandle, this, &UBuildingManagerBase::ROC_RunConstructionEvent, BaseConstructionSpeed, true);
		}
	}
	// delete unneeded handle
	else
	{
		// check if the timer handle exists
		if (ConstructionTimerHandle.IsValid())
		{
			// delete it if it does.
			GetWorld()->GetTimerManager().ClearTimer(ConstructionTimerHandle);
		}
	}
}

void UBuildingManagerBase::ROC_RunConstructionEvent_Implementation()
{
	if (ListOfUnderConstructionBuildings.Num() >= 1 && SpeedMutlipler != 0)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Construction Arrry Size: %d"), ListOfUnderConstructionBuildings.Num());
#endif
		// Set local array for removing constructed buildings.
		TArray<ABuildingMasterBase*> BuildingsToRemove;

		// go through all the buildings
		for (ABuildingMasterBase* TargetBuilding : ListOfUnderConstructionBuildings)
		{
			// check contruction status AND check do they have engineer units
			if (TargetBuilding->GetConstructionStatus() == EConstructionStatus::UnderConstruction) // add an && for engineers later
			{
				if (TargetBuilding->GetClass()->ImplementsInterface(UBuildingInterface::StaticClass()))
				{
					// calculate the amount to add. 
					float DeltaConstructionTimeLocal = BaseConstructionSpeed / SpeedMutlipler; // add in number of characters later
#if WITH_EDITOR
					UE_LOG(LogTemp, Warning, TEXT("Delta Construction: %f"), DeltaConstructionTimeLocal);
#endif
					IBuildingInterface::Execute_TriggerAddConstructedTime(TargetBuilding, DeltaConstructionTimeLocal);
				}
				// Check if mesh updated needed.
				if (TargetBuilding->GetBuildingData().ConstructionMeshes.Num() == 4)
				{
					// 0-24% is skipped as this is the default
					// 25-49%
					if ((TargetBuilding->GetTimeConstructed() / TargetBuilding->GetConstructionTime()) >= .25f && (TargetBuilding->GetTimeConstructed() / TargetBuilding->GetConstructionTime()) < .50f && TargetBuilding->GetBuildingStatus() != 1)
					{
						IBuildingInterface::Execute_TriggerSetBuildingStatus(TargetBuilding, 1);
					}
					// 50-74 %
					if ((TargetBuilding->GetTimeConstructed() / TargetBuilding->GetConstructionTime()) >= .50f && (TargetBuilding->GetTimeConstructed() / TargetBuilding->GetConstructionTime()) < .75f && TargetBuilding->GetBuildingStatus() != 2)
					{
						IBuildingInterface::Execute_TriggerSetBuildingStatus(TargetBuilding, 2);
					}
					// 75+ %
					if ((TargetBuilding->GetTimeConstructed() / TargetBuilding->GetConstructionTime()) >= .75f && TargetBuilding->GetBuildingStatus() != 3)
					{
						IBuildingInterface::Execute_TriggerSetBuildingStatus(TargetBuilding, 3);
					}
				}
				else if (TargetBuilding->GetBuildingData().ConstructionMeshes.Num() == 3)
				{
					// 0-33% is skipped as this is the default
					// 33-65%
					if ((TargetBuilding->GetTimeConstructed() / TargetBuilding->GetConstructionTime()) >= .33f && (TargetBuilding->GetTimeConstructed() / TargetBuilding->GetConstructionTime()) < .66f && TargetBuilding->GetBuildingStatus() != 1)
					{
						IBuildingInterface::Execute_TriggerSetBuildingStatus(TargetBuilding, 1);
					}
					// 66% +
					else if ((TargetBuilding->GetTimeConstructed() / TargetBuilding->GetConstructionTime()) >= .66f && TargetBuilding->GetBuildingStatus() != 2)
					{
						IBuildingInterface::Execute_TriggerSetBuildingStatus(TargetBuilding, 2);
					}
				}
				else if (TargetBuilding->GetBuildingData().ConstructionMeshes.Num() == 2)
				{
					// 50%+
					if ((TargetBuilding->GetTimeConstructed() / TargetBuilding->GetConstructionTime()) >= .50f && TargetBuilding->GetBuildingStatus() != 1)
					{
						IBuildingInterface::Execute_TriggerSetBuildingStatus(TargetBuilding, 1);
					}
				}
#if WITH_EDITOR
				UE_LOG(LogTemp, Warning, TEXT("Constructed Rate: %f"), (TargetBuilding->GetTimeConstructed() / TargetBuilding->GetConstructionTime()));
#endif

				// check if completed.
				if (TargetBuilding->GetTimeConstructed() >= TargetBuilding->GetConstructionTime())
				{
					// buildings are now completed.
					// add to list of indices targetted for removeal from ListOfUnderConstructionBuildings.
					BuildingsToRemove.Add(TargetBuilding);
					// update construction status
					if (TargetBuilding->GetBuildingData().ConstructionMeshes.Num() == 1)
					{
						IBuildingInterface::Execute_TriggerSetConstructionStatus(TargetBuilding, EConstructionStatus::Built);
						IBuildingInterface::Execute_TriggerSetBuildingStatus(TargetBuilding, -1);
					}
					else
					{
						IBuildingInterface::Execute_TriggerSetConstructionStatus(TargetBuilding, EConstructionStatus::Built);
						IBuildingInterface::Execute_TriggerSetBuildingStatus(TargetBuilding, 0);
					}
#if WITH_EDITOR
					UE_LOG(LogTemp, Warning, TEXT("BUILDING COMPLETED!"));
#endif
				}
			}
		}
		// go through the array of indices to remove to remove them from the list of under construction buildings
		if (BuildingsToRemove.Num() >= 0)
		{
			for (ABuildingMasterBase* CompletedBuilding : BuildingsToRemove)
			{
				ROC_RemoveToUnconstructedList(CompletedBuilding);
				ROC_AddToListOfBuildings(CompletedBuilding);
				if (ListOfUnderConstructionBuildings.Num() < 1)
				{
					GetWorld()->GetTimerManager().ClearTimer(ConstructionTimerHandle);
				}
			}
			// clear the array, despite it being local, to be on the safe side.
			BuildingsToRemove.Empty();
		}
	}
	// this should not be firing if the array is empty. Invalidate the timer.
	else if(ListOfUnderConstructionBuildings.Num() < 1)
	{
		GetWorld()->GetTimerManager().ClearTimer(ConstructionTimerHandle);
	}
}

void UBuildingManagerBase::ROC_AddToListOfBuildings_Implementation(ABuildingMasterBase* NewBuilding)
{
	if (!ensure(NewBuilding != nullptr)) return;
	ListOfBuildings.AddUnique(NewBuilding);
}

void UBuildingManagerBase::ROC_RemoveToListOfBuildings_Implementation(ABuildingMasterBase* DestroyedBuilding)
{
	if (!ensure(DestroyedBuilding != nullptr)) return;

	if (ListOfBuildings.Num() >= 1)
	{
		ListOfBuildings.RemoveSingle(DestroyedBuilding);
	}
}

/************************************************
* COMBAT AND HEALTH RELATED
************************************************/

void UBuildingManagerBase::RunDamageEvent(ABuildingMasterBase* TargetBuilding, float DamageAmount)
{
	if (!ensure(TargetBuilding != nullptr)) return;

	bool bBuildingDestroyed = DamageBuilding(TargetBuilding, DamageAmount);
	if (bBuildingDestroyed)
	{
		ROC_DestroyBuilding(TargetBuilding);
	}
}

bool UBuildingManagerBase::DamageBuilding(ABuildingMasterBase* TargetBuilding, float DamageAmount)
{
	if (!ensure(TargetBuilding != nullptr)) return false;

	float NewStabilitiy = TargetBuilding->GetStability().MinValue - DamageAmount;
	if (TargetBuilding->GetClass()->ImplementsInterface(UBuildingInterface::StaticClass()))
	{
		IBuildingInterface::Execute_TriggerSetBuildingMinStability(TargetBuilding, NewStabilitiy);
	}

	return NewStabilitiy <= 0;
}

void UBuildingManagerBase::ROC_DestroyBuilding_Implementation(ABuildingMasterBase* TargetBuilding)
{
	if (!ensure(TargetBuilding != nullptr)) return;

	ROC_RemoveToListOfBuildings(TargetBuilding);
	IBuildingInterface::Execute_TriggerSetIsDestroyed(TargetBuilding, true);
}

/************************************************
* GETTERS
************************************************/
FVector UBuildingManagerBase::GetGridSnaps()
{
	float XLocal = (((floor(MousePosition.X + 50.0f)) / SnapValue) * SnapValue);
	float YLocal = (((floor(MousePosition.Y + 50.0f)) / SnapValue) * SnapValue);
	return FVector(XLocal, YLocal, MousePosition.Z);
}
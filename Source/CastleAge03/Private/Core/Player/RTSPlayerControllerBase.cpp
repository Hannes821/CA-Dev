// RTS Player controller, written by Zachary J. Parker (2021), adapted from earlier work.
// RTS player controller code may be used for this project and has been previously used by the above in a tutorial.

#define LANDSCAPEChannel ECC_GameTraceChannel2

#include "Core/Player/RTSPlayerControllerBase.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Core/Components/CameraMovementComponent.h"

// Constructor
ARTSPlayerControllerBase::ARTSPlayerControllerBase()
{
	// Set this pawn to call tick() - can be turned off as needed
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	bShouldPerformFullTickWhenPaused = true;

	bReplicates = true;

	BuildingMangerRef = CreateDefaultSubobject<UBuildingManagerBase>(TEXT("BuildingManager"));
	if (BuildingMangerRef)
	{
		BuildingMangerRef->SetNetAddressable();
		BuildingMangerRef->SetIsReplicated(true);
	}
	
	ResourceManagerRef = CreateDefaultSubobject<UResourceManagerBase>(TEXT("ResourceManager"));
	if (ResourceManagerRef)
	{
		ResourceManagerRef->SetNetAddressable();
		ResourceManagerRef->SetIsReplicated(true);
	}


}

//Begin play
void ARTSPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(DelayStartTimerHandle, this, &ARTSPlayerControllerBase::DelayedBeginPlay, 3.2f, true);

	// Show mouse
	bShowMouseCursor = true;

	//Widgets will be spawned in via the BP Child of this class file.

}

//Tick
void ARTSPlayerControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CameraMovementRef != nullptr)
	{
		CameraMovementRef->EdgeScroll(); // Edge scroll is here, unless I can create a dynmaic input for MouseX/MouseY.
		SetMousePosition(CameraPawnRef->GetCurrentArmLength()+ 2000.0f);
		BuildingMangerRef->SetMousePosition(this->GetCursorPosition());
	}
}

//Player controls (input)
void ARTSPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	//setup gameplay key bindings
	check(InputComponent);

	/************************************************
	* CAMERA MOVEMENT CONTROLS
	************************************************/
	// MoveFoward
	InputComponent->BindAxis("MoveFoward", this, &ARTSPlayerControllerBase::CallMoveFoward);
	// MoveRight
	InputComponent->BindAxis("MoveRight", this, &ARTSPlayerControllerBase::CallMoveRight);
	//SpeedModifier (increase)
	InputComponent->BindAction("SpeedModifier", IE_Pressed, this, &ARTSPlayerControllerBase::CallIncraseSpeedModifier);
	//SpeedModifier (decrease)
	InputComponent->BindAction("SpeedModifier", IE_Released, this, &ARTSPlayerControllerBase::CallDecreaseSpeedModifier);

	//ZoomIn
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ARTSPlayerControllerBase::CallZoomIn);
	//ZoomOut
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ARTSPlayerControllerBase::CallZoomOut);
	//ZoomReset
	InputComponent->BindAction("ZoomReset", IE_Pressed, this, &ARTSPlayerControllerBase::CallZoomReset);

	//PanReset
	InputComponent->BindAction("PanReset", IE_Pressed, this, &ARTSPlayerControllerBase::CallPanReset);

	//CameraReset
	InputComponent->BindAction("ResetCamera", IE_Pressed, this, &ARTSPlayerControllerBase::CallCameraReset);

	/************************************************
	* Player Controller Actions 
	(e.g., construction, unit selection...)
	************************************************/

	InputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ARTSPlayerControllerBase::PrimaryActionSwitcher);

	/************************************************
	* CONSTRUCTION/BUILDING CONTROLS
	************************************************/
	// Rotate Previews (left and right)
	InputComponent->BindAction("RotatePreviewLeft", IE_Pressed, this, &ARTSPlayerControllerBase::CallRotatePreviewLeft);
	InputComponent->BindAction("RotatePreviewLeft", IE_Released, this, &ARTSPlayerControllerBase::CallEndRotatePreivew);
	InputComponent->BindAction("RotatePreviewRight", IE_Pressed, this, &ARTSPlayerControllerBase::CallRotatePreviewRight);
	InputComponent->BindAction("RotatePreviewRight", IE_Released, this, &ARTSPlayerControllerBase::CallEndRotatePreivew);
}

/************************************************
* CAMERA MOVEMENT CONTROLS
************************************************/
void ARTSPlayerControllerBase::DelayedBeginPlay()
{
	// Refernece casts (may replace these the first with an interface, BUT the camera pawn is 'friends of class' to this controller)
	CameraPawnRef = Cast<ACameraPawn>(GetPawn()); // gets camera pawn - the custom one.
	CameraMovementRef = CameraPawnRef->PawnMovementComponent; // Gets the movement component of the camera pawn, accessed via the CameraPawnRef via an interface call.

	if (HasAuthority())
	{
		Possess(CameraPawnRef);
		if (GetWorld()->GetGameState()->GetClass()->ImplementsInterface(UGameStateInterface::StaticClass()))
		{
			IGameStateInterface::Execute_AddPlayerController(GetWorld()->GetGameState(), GetWorld()->GetFirstPlayerController());
		}
	}
	GetWorld()->GetTimerManager().ClearTimer(DelayStartTimerHandle);
}

void ARTSPlayerControllerBase::SendIdentityToServer(APlayerController* SelfRef) // NOT USED...
{
	if (GetWorld()->GetGameState()->GetClass()->ImplementsInterface(UGameStateInterface::StaticClass()))
	{
		IGameStateInterface::Execute_AddPlayerController(GetWorld()->GetGameState(), SelfRef);
	}
}

// Move Forward - What happens when W/S is pressed
void ARTSPlayerControllerBase::CallMoveFoward(float value)
{
	if (CameraMovementRef != nullptr)
	{
		CameraMovementRef->BasicMovementControl(value, 0.0);
	}
}

// Move Right - what happens when A/D is pressed
void ARTSPlayerControllerBase::CallMoveRight(float value)
{
	if (CameraMovementRef != nullptr)
	{
		CameraMovementRef->BasicMovementControl(0.0, value);
	}
}

// Increase movement speed - what happens when shift is pressed
void ARTSPlayerControllerBase::CallIncraseSpeedModifier()
{
	if (CameraMovementRef != nullptr)
	{
		CameraMovementRef->SetMovementSpeedModifier(2.0f);
	}
}

// Decrease movement speed - what happens when shift is released
void ARTSPlayerControllerBase::CallDecreaseSpeedModifier()
{
	if (CameraMovementRef != nullptr)
	{
		CameraMovementRef->SetMovementSpeedModifier(1.0f);
	}
}

void ARTSPlayerControllerBase::CallZoomIn()
{
	if (CameraMovementRef != nullptr)
	{
		CameraMovementRef->ZoomIn();
	}
}

void ARTSPlayerControllerBase::CallZoomOut()
{
	if (CameraMovementRef != nullptr)
	{
		CameraMovementRef->ZoomOut();
	}
}

void ARTSPlayerControllerBase::CallZoomReset()
{
	if (CameraMovementRef != nullptr)
	{
		CameraMovementRef->ResetZoom();
	}
}

void ARTSPlayerControllerBase::CallPanReset()
{
	if (CameraMovementRef != nullptr)
	{
		CameraMovementRef->ResetPan();
	}
}

void ARTSPlayerControllerBase::CallCameraReset()
{
	if (CameraMovementRef != nullptr)
	{
		CameraMovementRef->CameraReset();
	}
}

void ARTSPlayerControllerBase::CallRotatePreviewLeft()
{
	CameraMovementRef->SetCameraDiabled(true);
	CameraMovementRef->SetDisableEdgeScroll(true);
	BuildingMangerRef->RotatePreviewLeft();
}

void ARTSPlayerControllerBase::CallRotatePreviewRight()
{
	CameraMovementRef->SetCameraDiabled(true);
	CameraMovementRef->SetDisableEdgeScroll(true);
	BuildingMangerRef->RotatePreviewRight();
}

void ARTSPlayerControllerBase::CallEndRotatePreivew()
{
	CameraMovementRef->SetCameraDiabled(false);
	CameraMovementRef->SetDisableEdgeScroll(false);
}

/************************************************
* Player Controller Actions
(e.g., construction, unit selection...)
************************************************/

void ARTSPlayerControllerBase::PrimaryActionSwitcher()
{
	switch (ActionMode)
	{
	case 0:
		// selection mode
		break;
	case 1: // construction mode
		ConstructionActions();
		break;
	case 2:
		// deletion mode
		break;
	case 3:
		// disabled. Literally break.
		break;
	//default:
		// selection mode
	}
}



/************************************************
* CONSTRUCTION/BUILDING CONTROLS
************************************************/

void ARTSPlayerControllerBase::ConstructionActions()
{
	//check if preview is valid
	if (BuildingMangerRef->GetPreviewBuilding() != nullptr)
	{
		// check if valid area to place building. 
		if (BuildingMangerRef->GetPreviewBuilding()->CheckIsOverlapping() != true && BuildingMangerRef->GetPreviewBuilding()->GetIsOnSlope() != true)
		{
			bool bHasEnoughResourcesLocal{ false };
			int CountLocal{ 0 };
			for (FBuildingCosts TargetData : BuildingMangerRef->GetPreviewBuilding()->GetBuildingData().BuildingCosts)
			{				
				if (ResourceManagerRef->GetCurrentResourceAmount(TargetData.Resource) >= TargetData.Amount)
				{
					CountLocal++;
					// bHasEnoughResourcesLocal = true;
				}
				else
				{
					CountLocal--;
					// bHasEnoughResourcesLocal = false;
				}
			}
			if (CountLocal >= BuildingMangerRef->GetPreviewBuilding()->GetBuildingData().BuildingCosts.Num())
			{
				bHasEnoughResourcesLocal = true;
			}
			else
			{
				bHasEnoughResourcesLocal = false;
			}

			// NOTE ON ROC_StoreVariable and ROS_SpawnConstruction
			// currently due to replication, data has to be passed from building manager to building manager
			// Otherwise this function would just be called and building manager could store the data itself
			// For some reason, building manager was not spawning correctly without these variables passed this way.
			// Will work on an update to see if this can be done in a more efficient and more OOP way.
			if (bHasEnoughResourcesLocal)
			{
				for (FBuildingCosts TargetData : BuildingMangerRef->GetPreviewBuilding()->GetBuildingData().BuildingCosts)
				{
					ResourceManagerRef->ROS_SetResourceAmount(TargetData.Resource, -TargetData.Amount);
				}
				BuildingMangerRef->ROC_StoreVariables(BuildingMangerRef->GetPreviewBuilding()->GetActorTransform(), BuildingMangerRef->GetPreviewBuilding()->GetRowName());
				BuildingMangerRef->ROC_DestroyPreview();
				// BuildingMangerRef->ROC_RunConstructionStart(BuildingMangerRef->GetTargetTransform(), BuildingMangerRef->GetRowName());
				BuildingMangerRef->ROS_SpawnConstruction(BuildingMangerRef->GetTargetTransform(), BuildingMangerRef->GetRowName());
				if (HasAuthority()) // TODO remove?
				{
					BuildingMangerRef->ROC_RunConstructionStart(BuildingMangerRef->GetTargetTransform(), BuildingMangerRef->GetRowName());
					// reduce player resources on construction of building

				}
				
							

#if WITH_EDITOR
				FName RowNameLocal = BuildingMangerRef->GetRowName();
				FVector LocationLocal = BuildingMangerRef->GetTargetTransform().GetLocation();
				UE_LOG(LogTemp, Warning, TEXT("SPAWNING %s at %s"), *RowNameLocal.ToString(), *LocationLocal.ToString());
#endif
			}
			else
			{
				BuildingMangerRef->ROC_DestroyPreview();
			}
		}
	}
	ROC_SetActionMode(EPrimaryActionModes::Selection);
}

FVector ARTSPlayerControllerBase::SetMousePosition(float TraceDistance)
{
	// Set params for lince trace
	FHitResult OutHitLocal;
	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = false;
	CollisionParams.AddIgnoredActor(this);

	// Create the values for start and end of trace
	FVector WorldLocationLocal; // this is start point
	FVector WorldDirectionLocal;
	DeprojectMousePositionToWorld(WorldLocationLocal, WorldDirectionLocal);
	FVector EndPointLocal = ((WorldDirectionLocal * TraceDistance) + WorldLocationLocal);

	// run line trace
	GetWorld()->LineTraceSingleByChannel(OutHitLocal, WorldLocationLocal, EndPointLocal, LANDSCAPEChannel, CollisionParams);

	if (OutHitLocal.HitObjectHandle != nullptr && OutHitLocal.HitObjectHandle.IsValid())
	{
		MousePosition = OutHitLocal.Location;
		return MousePosition;
	}

	return FVector();
}

bool ARTSPlayerControllerBase::SetGameDateTime_Implementation(FDateTime TimeStruct)
{
	RoC_PassDateTimeStruct(TimeStruct);
	return true;
}

bool ARTSPlayerControllerBase::SetGameDay_Implementation(int GameDay)
{
	RoC_PassDayOfWeek(GameDay);
	return true;
}

void ARTSPlayerControllerBase::RoC_PassDateTimeStruct_Implementation(FDateTime TimeStruct)
{
	if (!ensure(MainUI != nullptr)) return;
	if (MainUI->GetClass()->ImplementsInterface(UGameTimeInterface::StaticClass()))
	{
		IGameTimeInterface::Execute_SetGameDateTime(MainUI, TimeStruct);
	}
}

void ARTSPlayerControllerBase::RoC_PassDayOfWeek_Implementation(int GameDay)
{
	if (!ensure(MainUI != nullptr)) return;
	if (MainUI->GetClass()->ImplementsInterface(UGameTimeInterface::StaticClass()))
	{
		IGameTimeInterface::Execute_SetGameDay(MainUI, GameDay);
	}
}

bool ARTSPlayerControllerBase::SetGameSpeedInt_Implementation(float GameSpeed)
{
	RoC_PassGameSpeed(GameSpeed);
	return true;
}

void ARTSPlayerControllerBase::RoC_PassGameSpeed_Implementation(float GameSpeed)
{
	if (!ensure(MainUI != nullptr)) return;
	if (MainUI->GetClass()->ImplementsInterface(UGameTimeInterface::StaticClass()))
	{
		IGameTimeInterface::Execute_SetGameSpeedInt(MainUI, GameSpeed);
	}
}

void ARTSPlayerControllerBase::RoS_SetGameSpeed_Implementation(float SpeedMutliplerLocal)
{
	if (GetWorld()->GetGameState()->GetClass()->ImplementsInterface(UGameTimeInterface::StaticClass()))
	{
		IGameTimeInterface::Execute_UpdateGameSpeed(GetWorld()->GetGameState(), SpeedMutliplerLocal);
	}
}

void ARTSPlayerControllerBase::ROC_SetActionMode_Implementation(EPrimaryActionModes Selection)
{
	ActionMode=Selection;
}

UBuildingManagerBase* ARTSPlayerControllerBase::GetBuildingManager_Implementation()
{
	if (!ensure(BuildingMangerRef != nullptr)) return nullptr;
	return BuildingMangerRef;
}

void ARTSPlayerControllerBase::TriggerSetActionMode_Implementation(EPrimaryActionModes Selection)
{
	ROC_SetActionMode(Selection);
}

EPrimaryActionModes ARTSPlayerControllerBase::GetActionMode_Implementation()
{
	return(ActionMode);
}

void ARTSPlayerControllerBase::TestSet(EResources Value)
{
	DebugValueFloat = FMath::RandRange(0, 10);
	ResourceManagerRef->ROS_SetResourceAmount(Value, DebugValueFloat);
}

void ARTSPlayerControllerBase::UpdateGameSpeed_Implementation(float SpeedMultiplier)
{
	RoS_SetGameSpeed(SpeedMultiplier);
}

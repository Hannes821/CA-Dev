// Code written by Zachary Parker for Feldfeste 

#define LANDSCAPEChannel ECC_GameTraceChannel2

#include "Core/Actors/BuildingMasterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Core/Library/GameTimeFunc.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ABuildingMasterBase::ABuildingMasterBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to replicate
	bReplicates = true;
	bAlwaysRelevant = true;

	/* Locate and load in the data table.*/
	BuildingsDataTable = LoadObject<UDataTable>(NULL, TEXT("/Game/Core/DataTables/DT_Buildings"));

	/* locate and load in preview materials*/
	FString sPreviewMaterialPath = "/Game/Buildings/Materials/Global/MI_BuildingPreview";
	PreviewMaterial = LoadMaterialFromPath(FName(*sPreviewMaterialPath));

	// Set root
	BuildingRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BuildingRoot"));
	SetRootComponent(BuildingRoot);

	// Set Collision and attach to root
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(BuildingRoot);

	// Set up static mesh, attach to root
	CurrentBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CurrentBuildingMesh"));
	CurrentBuildingMesh->SetupAttachment(BuildingRoot);

	// Set entry point up
	EntryPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("EntryPoint"));
	EntryPoint->SetupAttachment(BuildingRoot);
}

// Replicated variables
void ABuildingMasterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABuildingMasterBase, BuildingData);
	DOREPLIFETIME(ABuildingMasterBase, RowName);
	DOREPLIFETIME(ABuildingMasterBase, BuildingRoot);
	DOREPLIFETIME(ABuildingMasterBase, CurrentBuildingMesh);
	DOREPLIFETIME(ABuildingMasterBase, CollisionBox);
	DOREPLIFETIME(ABuildingMasterBase, EntryPoint);
	DOREPLIFETIME(ABuildingMasterBase, BuildingsDataTable);
	DOREPLIFETIME(ABuildingMasterBase, BuildingOwnerRef);
	DOREPLIFETIME(ABuildingMasterBase, MaterialList);
	DOREPLIFETIME(ABuildingMasterBase, Extent);
	DOREPLIFETIME(ABuildingMasterBase, ConstructionStatus);
	DOREPLIFETIME(ABuildingMasterBase, BuildingStatus);
	DOREPLIFETIME(ABuildingMasterBase, ConstructedTime);
}


/************************************************
*    CONSTRUCTION SCRIPTS (not constructor) 
*                     &
*           BeginPlay! (don't forget tick)
************************************************/

void ABuildingMasterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetMesh();
}

void ABuildingMasterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	/*
	* Set Building Mesh
	* When called in game or placed in world, Row name will be set.
	* With the name set the building mesh can be set.
	* Prevents undeclared buildings from existing (second failsafe will be in begin play)
	*/

	SetMesh();
}

// Called when the game starts or when spawned
void ABuildingMasterBase::BeginPlay()
{
	Super::BeginPlay();

	// Destroy actor if there is no building data else set mesh
	if (RowName == "Null")
	{
		Destroy();
	}
	// else seet the correct mesh.
	else
	{
		switch (ConstructionStatus)
		{
		case 0: // preview mode
			if (BuildingOwnerRef == GetWorld()->GetFirstPlayerController())
			{
				ROC_SetPreviewMesh();
			}
			break;
		case 1: // under construction
			ROS_SetConstructionMesh();
			SetMesh();
			break;
		case 2:// built
			ROS_SetMesh();
			break;
		default:
			Destroy();
			break;
		}
	}

	// check if the actor is pending destroy, if not bind to the game state for the game speed delegate
	//if (!IsPendingKillPending())
	//{ 
	//	GetWorld()->GetGameState<ARTSGameState>()->GameSpeedControl.BindUObject(this, &ABuildingMasterBase::UpdateGameSpeed);
//	}
}

// Called every frame
void ABuildingMasterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/************************************************
* GLOBAL BUILDING RELATED FUNCTIONS
************************************************/

void ABuildingMasterBase::ROS_SetMesh_Implementation(int Index)
{
	SetMesh(Index);
}

void ABuildingMasterBase::ROC_SetMesh_Implementation(int Index)
{
	SetMesh(Index);
}

void ABuildingMasterBase::SetMesh(int Index)
{
	// Check if a row name has been provided
	if (RowName != "Null")
	{
		// check if the data table is valid
		if (BuildingsDataTable == nullptr)
		{
			/* if not, then locate and load in the data table.*/
			BuildingsDataTable = LoadObject<UDataTable>(NULL, TEXT("/Game/Core/DataTables/DT_Buildings"));
		}

		// ensure the data table is loaded.
		if (!ensure(BuildingsDataTable != nullptr)) return;

		//find the row and confirm it exists
		FBuildingStruct* BuildingDataLocal = BuildingsDataTable->FindRow<FBuildingStruct>(RowName, "", false);
		bool Success = BuildingDataLocal ? true : false;

		// If the row exists, set the current mesh and building data.
		if (Success == true)
		{
			BuildingData = *BuildingDataLocal; // store the building data.
			switch (ConstructionStatus)
			{
			case 0: //preview
				CurrentBuildingMesh->SetStaticMesh(BuildingData.BaseMeshes[Index]);
				break;
			case 1: // Construction
				CurrentBuildingMesh->SetStaticMesh(BuildingData.ConstructionMeshes[Index]);
				break;
			case 2: // built
				CurrentBuildingMesh->SetStaticMesh(BuildingData.BaseMeshes[Index]);
				break;
			default:
				CurrentBuildingMesh->SetStaticMesh(BuildingData.BaseMeshes[Index]);
			}
			SetCollisionArea();
			SetEntrance();
			Extent = CollisionBox->GetScaledBoxExtent() + CollisionBox->GetScaledBoxExtent();
		}
		// if the row does not exist, destroy this instance.
		else
		{
			Destroy();
		}
	}
}

void ABuildingMasterBase::SetCollisionArea()
{
	if (CurrentBuildingMesh)
	{
		// declare local variables
		FVector MinLocal;
		FVector MaxLocal;
		FVector NewExtentLocal;

		// Get and store the local bound of the static mesh in the Min and Max Local
		CurrentBuildingMesh->GetLocalBounds(MinLocal, MaxLocal);

		// Set the FVector for the box extent.
		NewExtentLocal = FVector(((FMath::Abs(MinLocal.X)) - 15.0f), ((FMath::Abs(MinLocal.Y)) - 15.0f), ((MaxLocal.Z) / 2));

		// Set the new box extent.
		CollisionBox->SetBoxExtent(NewExtentLocal, true);

		// Set relative location of the collision area.
		CollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, NewExtentLocal.Z), false); // if buildings are floating look here!

	}
}

void ABuildingMasterBase::SetEntrance()
{
	if (CurrentBuildingMesh)
	{
		// declare local variables
		FVector MinLocal;
		FVector MaxLocal;
		FVector NewExtentLocal;

		// Get and store the local bound of the static mesh in the Min and Max Local
		CurrentBuildingMesh->GetLocalBounds(MinLocal, MaxLocal);

		// Set the FVector for the box extent.
		NewExtentLocal = FVector(98.0f, (FMath::Abs(MinLocal.Y) - 20.0f), 10.0f);

		// Set new box extent.
		EntryPoint->SetBoxExtent(NewExtentLocal, true);

		// ster relative location of the entrance
		EntryPoint->SetRelativeLocation(FVector(((FMath::Abs(MinLocal.X) + 150) * -1), 0.0f, 10.0f), false);
	}
}

void ABuildingMasterBase::ROS_SetBuildingOwner_Implementation(APlayerController* OwnerIn)
{
	BuildingOwnerRef = OwnerIn;
	SetOwner(OwnerIn);
}

void ABuildingMasterBase::ROS_SetConstructionStatus_Implementation(EConstructionStatus Status)
{
	ConstructionStatus = Status;
}

void ABuildingMasterBase::ROS_SetBuildingStatus_Implementation(int Value)
{
	BuildingStatus = Value;
	// Make sure the host runs the events too.
	if (HasAuthority())
	{
		OnRep_UpdateBuildingStatus();
	}
}

void ABuildingMasterBase::OnRep_UpdateBuildingStatus()
{
	int IndexValueLocal = BuildingStatus;
	if (ConstructionStatus == EConstructionStatus::Preview)
	{ // do nothing on server if preview
		return;
	}
	else if (ConstructionStatus == EConstructionStatus::UnderConstruction)
	{
		ROS_SetMesh(IndexValueLocal);
		ROC_SetMesh(IndexValueLocal);
	}
	else if (IndexValueLocal == -1 && ConstructionStatus == EConstructionStatus::Built)
	{
		/* This is for case where the there is 1 construction mesh (index 0) and the construction has finished. */
		IndexValueLocal = 0;
		ROS_SetMesh(IndexValueLocal);
		ROC_SetMesh(IndexValueLocal);
	}
	else if (ConstructionStatus == EConstructionStatus::Built)
	{
		ROS_SetMesh(IndexValueLocal);
		ROC_SetMesh(IndexValueLocal);
	}
}

//void ABuildingMasterBase::UpdateGameSpeed(float inGameSpeed)
//{
//	GameSpeed = inGameSpeed;
//	int SpeedSettingLocal = UGameTimeFunc::GetGameSpeedInt(GameSpeed);
//	SpeedMutlipler = UGameTimeFunc::SetSpeedMutliplerLocal(SpeedSettingLocal);
//#if WITH_EDITOR
//	UE_LOG(LogTemp, Warning, TEXT("Building Speed Multiplier is %f"), SpeedMutlipler);
//#endif
//}

/************************************************
* PREVIEW/GHOST BUILDING METHODS
************************************************/

void ABuildingMasterBase::DestroyPreview()
{
	CurrentBuildingMesh->SetStaticMesh(NULL);
	SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	Destroy();
}

void ABuildingMasterBase::ROC_SetPreviewMesh_Implementation()
{
	ROS_SetConstructionStatus(EConstructionStatus::Preview);
	SetMesh(); // default is fine here.
}

void ABuildingMasterBase::ROC_SetPreviewMaterial_Implementation()
{
	// check if the preview material is loaded in.
	if (!ensure(PreviewMaterial != nullptr)) return;

	int LastIndexLocal = CurrentBuildingMesh->GetNumMaterials();
	for (int i = 0; i < LastIndexLocal; i++)
	{
		// Set all mesh materials to preview
		CurrentBuildingMesh->SetMaterial(i, PreviewMaterial);
		MaterialList.AddUnique(CurrentBuildingMesh->CreateDynamicMaterialInstance(i, PreviewMaterial));
	}
}

void ABuildingMasterBase::MovePreview(FVector NewLocation)
{
#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("Building Location: %s"), *NewLocation.ToString());
#endif
	SetActorLocation(NewLocation, false);
	ROC_SetMaterialOverlap();
}

void ABuildingMasterBase::ROC_RotatePreview_Implementation(FRotator NewRotation)
{
	SetActorRotation(NewRotation);
}

void ABuildingMasterBase::ROC_SetMaterialOverlap_Implementation()
{
	// if overlapping or on slope
	if (CheckIsOverlapping() == true || CalculateSlope() == true)
	{
		for (UMaterialInstanceDynamic* MaterialLocal : MaterialList)
		{
			// is overlapping set to red
			MaterialLocal->SetVectorParameterValue(FName("Colour"), FLinearColor(1.0f, 0.0f, 0.0f, 0.75f));
		}
	}
	// if NOT overlapping or NOT on slope
	else if (CheckIsOverlapping() == false && CalculateSlope() == false)
	{
		for (UMaterialInstanceDynamic* MaterialLocal : MaterialList)
		{
			// is NOT overlapping. Set to to default
			MaterialLocal->SetVectorParameterValue(FName("Colour"), FLinearColor(0.22f, 0.56f, 0.72f, 1.0f));
		}
	}
}

float ABuildingMasterBase::SlopeTrace(FVector TraceLocation)
{
	// Set params for lince trace
	FHitResult OutHitLocal;
	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = false;
	CollisionParams.AddIgnoredActor(this);

	//run line trace
	GetWorld()->LineTraceSingleByChannel(OutHitLocal, (TraceLocation + FVector(0.0f, 0.0f, 1000.0f)), (TraceLocation - FVector(0.0f, 0.0f, 1000.0f)), LANDSCAPEChannel, CollisionParams);

	return GetActorLocation().Z - OutHitLocal.ImpactPoint.Z;;
}

bool ABuildingMasterBase::CalculateSlope()
{
	/* Set up for the function*/
	// Set variables used throughout this calculation
	FVector ActorLocationLocal = GetActorLocation();
	FVector ForwardVectorLocal = GetActorForwardVector();
	FVector RightVectorLocal = GetActorRightVector();

	// Clear the array to avoid issues
	Differences.Empty();

	/* contact edge calculations - determine where mesh and ground meet. */
	// four corners
	Differences.Add(SlopeTrace((ActorLocationLocal + ((ForwardVectorLocal + RightVectorLocal) * Extent))));
	Differences.Add(SlopeTrace((ActorLocationLocal + (((ForwardVectorLocal * -1.0f) + RightVectorLocal) * Extent))));
	Differences.Add(SlopeTrace((ActorLocationLocal + ((ForwardVectorLocal + (RightVectorLocal * -1.0f)) * Extent))));
	Differences.Add(SlopeTrace((ActorLocationLocal + (((ForwardVectorLocal * -1.0f) + (RightVectorLocal * -1.0f)) * Extent))));

	// Origin
	Differences.Add(SlopeTrace(ActorLocationLocal));

	// Get and store variables for next two steps.
	int32 IndexMinLocal;
	float MinValueLocal;

	int32 IndexMaxLocal;
	float MaxValueLocal;

	UKismetMathLibrary::MinOfFloatArray(Differences, IndexMinLocal, MinValueLocal);
	UKismetMathLibrary::MaxOfFloatArray(Differences, IndexMaxLocal, MaxValueLocal);

	/* Set Mesh Height; based off landscape */
	if (IndexMinLocal > -1 && IndexMaxLocal > -1)
	{
		// Set params for lince trace
		FHitResult OutHitLocal;
		FCollisionQueryParams CollisionParams;
		CollisionParams.bTraceComplex = false;
		CollisionParams.AddIgnoredActor(this);

		bool bHasHit = GetWorld()->SweepSingleByChannel(OutHitLocal, (ActorLocationLocal + FVector(0.0f, 0.0f, 1000.0f)), (ActorLocationLocal - FVector(0.0f, 0.0f, 1000.0f)), FQuat::Identity, LANDSCAPEChannel, FCollisionShape::MakeBox(FVector(Extent.X, Extent.Y, 1.0)), CollisionParams);
		if (bHasHit)
		{
			SetActorLocation(FVector(ActorLocationLocal.X, ActorLocationLocal.Y, OutHitLocal.ImpactPoint.Z), false);
		}
	}

	/* Determine if the difference is acceeptable(if edges are greater than max, or away from landscape, or below landscape - then cannot place building) */
	// if true = bad location.
	bIsOnSlope = (FMath::Abs(MinValueLocal) > 20 || FMath::Abs(MaxValueLocal) > 20);
	return (FMath::Abs(MinValueLocal) > 20 || FMath::Abs(MaxValueLocal) > 20);
}

/************************************************
* CONSTRUCTION RELATED FUNCTIONS
************************************************/

void ABuildingMasterBase::ROS_SetConstructionMesh_Implementation()
{
	// Check if a row name has been provided
	if (RowName != "Null")
	{
		// check if the data table is valid
		if (BuildingsDataTable == nullptr)
		{
			/* if not, then locate and load in the data table.*/
			BuildingsDataTable = LoadObject<UDataTable>(NULL, TEXT("/Game/Core/DataTables/DT_Buildings"));
		}

		// ensure the data table is loaded.
		if (!ensure(BuildingsDataTable != nullptr)) return;

		//find the row and confirm it exists
		FBuildingStruct* BuildingDataLocal = BuildingsDataTable->FindRow<FBuildingStruct>(RowName, "", false);
		bool Success = BuildingDataLocal ? true : false;

		// If the row exists, set the current mesh and building data.
		if (Success == true)
		{
			BuildingData = *BuildingDataLocal;
			CurrentBuildingMesh->SetStaticMesh(BuildingData.ConstructionMeshes[0]);
			ROC_SetMesh();
			SetCollisionArea();
			SetEntrance();
		}
		// if the row does not exist, destroy this instance.
		else
		{
			Destroy();
		}
	}
}

/************************************************
* COMBAT AND HEALTH RELATED
************************************************/

void ABuildingMasterBase::ROS_SetStabilityMin_Implementation(float NewStability)
{
	BuildingData.Stability.MinValue = NewStability;

#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("Stability is %f / %f"), BuildingData.Stability.MaxValue, BuildingData.Stability.MinValue);
#endif
}

void ABuildingMasterBase::ROS_SetIsDestroyed_Implementation(bool Value)
{
	bIsDestroyed = Value;
	// esnure the host runs the event too
	if (HasAuthority())
	{
		OnRep_IsDestroyed();
	}
}

void ABuildingMasterBase::OnRep_IsDestroyed()
{
	// check this is not being called by accident
	if (bIsDestroyed)
	{
		// deal with occupants, other loses, etc.
		Destroy();
	}
}

/************************************************
* COMPLETED BUILDING RELATED FUNCTIONS
************************************************/

/************************************************
* INTERFACE RELATED (PREVIEW)
************************************************/

void ABuildingMasterBase::TriggerDestroyPreview_Implementation()
{
	DestroyPreview();
}

void ABuildingMasterBase::TriggerSetRowName_Implementation(FName TargetRowName)
{
	RowName = TargetRowName;
}

void ABuildingMasterBase::TriggerSetOwner_Implementation(APlayerController* TargetOwner)
{
	ROS_SetBuildingOwner(TargetOwner);
}

void ABuildingMasterBase::TriggerSetPreviewMaterial_Implementation()
{
	ROC_SetPreviewMaterial();
}

void ABuildingMasterBase::TriggerMovePreview_Implementation(FVector InNewLocation)
{
	MovePreview(InNewLocation);
}

void ABuildingMasterBase::TriggerRotatePreview_Implementation(FRotator NewRotation)
{
	ROC_RotatePreview(NewRotation);
}

void ABuildingMasterBase::TriggerSetConstructionStatus_Implementation(EConstructionStatus Status)
{
	ROS_SetConstructionStatus(Status);
}

void ABuildingMasterBase::TriggerAddConstructedTime_Implementation(float DeltaConstructionTime)
{
	// this value needs to match the timer value in BuildingManagerBase or the events will not be sinked.
	ConstructedTime += DeltaConstructionTime;
}

void ABuildingMasterBase::TriggerSetMesh_Implementation(int Index)
{
	ROC_SetMesh(Index);
}

void ABuildingMasterBase::TriggerSetBuildingStatus_Implementation(int Value)
{
	ROS_SetBuildingStatus(Value);
}

void ABuildingMasterBase::TriggerSetBuildingMinStability_Implementation(float NewStability)
{
	ROS_SetStabilityMin(NewStability);
}

void ABuildingMasterBase::TriggerSetIsDestroyed_Implementation(bool Value)
{
	ROS_SetIsDestroyed(Value);
}

/************************************************
* GETTERS
************************************************/
bool ABuildingMasterBase::CheckIsOverlapping()
{
	TArray<AActor*> OverlappingActorsLocal;
	GetOverlappingActors(OverlappingActorsLocal);
	
	return OverlappingActorsLocal.Num() > 0;
}




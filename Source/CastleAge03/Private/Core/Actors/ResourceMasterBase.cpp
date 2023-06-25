// Code written by Zachary Parker for Feldfeste 


#include "Core/Actors/ResourceMasterBase.h"
#include "Core/Settings/RTSGameState.h"

// Sets default values
AResourceMasterBase::AResourceMasterBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to replicate
	bReplicates = true;
	bAlwaysRelevant = true;

	/* Locate and load in the data table.*/
	ResourceDataTable = LoadObject<UDataTable>(NULL, TEXT("/Game/Core/DataTables/DT_Resources"));

	// Set root
	ResourceRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ResourceRoot"));
	SetRootComponent(ResourceRoot);

	ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResourceMesh"));
	ResourceMesh->SetupAttachment(ResourceRoot);
	ResourceMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));

	// Set Collision and attach to root
	HarvestArea = CreateDefaultSubobject<UBoxComponent>(TEXT("HarvestArea"));
	HarvestArea->SetupAttachment(ResourceRoot);
}

// Replicated variables
void AResourceMasterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AResourceMasterBase, ResourceRoot);
	DOREPLIFETIME(AResourceMasterBase, ResourceMesh);
	DOREPLIFETIME(AResourceMasterBase, HarvestArea);
	DOREPLIFETIME(AResourceMasterBase, ResourceDataTable);
	DOREPLIFETIME(AResourceMasterBase, ResourceData);
	DOREPLIFETIME(AResourceMasterBase, bAdjustCollisionOffset);
	DOREPLIFETIME(AResourceMasterBase, CustomCollisionSize);
	DOREPLIFETIME(AResourceMasterBase, RowName);
	DOREPLIFETIME(AResourceMasterBase, Season);
}

void AResourceMasterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ROS_UpdateMesh();
}

void AResourceMasterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ROS_UpdateMesh();
}

// Called when the game starts or when spawned
void AResourceMasterBase::BeginPlay()
{
	Super::BeginPlay();
	if (RowName == "NULL")
	{
		Destroy();
	}
	if (ResourceData.DepositType != EDepositeType::Building)
	{
		ROS_UpdateMesh();
	}

	
}

// Called every frame
void AResourceMasterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AResourceMasterBase::ROS_SetSeason_Implementation()
{
	ARTSGameState* GameStateLocal = GetWorld()->GetGameState<ARTSGameState>();
	if (GameStateLocal->GetClass()->ImplementsInterface(UGameTimeInterface::StaticClass()))
	{
		Season = IGameTimeInterface::Execute_GetSeason(GameStateLocal);
	}
}

void AResourceMasterBase::ROS_SetResourceData_Implementation()
{
	// ensure the data table is loaded.
	if (!ensure(ResourceDataTable != nullptr)) return;

	FResourceStruct* ResourceDataLocal = ResourceDataTable->FindRow<FResourceStruct>(RowName, "", false);
	bool Success = ResourceDataLocal ? true : false;

	if (Success)
	{
		ResourceData = *ResourceDataLocal;
	}
	else
	{
		Destroy();
	}
}

void AResourceMasterBase::ROS_UpdateMesh_Implementation()
{
	// ROS_SetSeason();
	if (RowName != "NULL")
	{
		if (ResourceDataTable == nullptr)
		{
			ResourceDataTable = LoadObject<UDataTable>(NULL, TEXT("/Game/Core/DataTables/DT_Resources"));
		}

		if (!ensure(ResourceDataTable != nullptr)) return;

		FResourceStruct* ResourceDataLocal = ResourceDataTable->FindRow<FResourceStruct>(RowName, "", false);
		bool Success = ResourceDataLocal ? true : false;

		// If the row exists, set the current mesh and resource data.
		if (Success)
		{
			ResourceData = *ResourceDataLocal; // store the resource data
		
			switch (Season)
			{
			case SeasonWinter:
				ResourceMesh->SetStaticMesh(ResourceData.BaseMesh[0]);
				break;
			case SeasonSpring:
				ResourceMesh->SetStaticMesh(ResourceData.BaseMesh[1]);
				break;
			case SeasonSummer:
				ResourceMesh->SetStaticMesh(ResourceData.BaseMesh[2]);
				break;
			case SeasonFall:
				ResourceMesh->SetStaticMesh(ResourceData.BaseMesh[3]);
				break;
			default:
				ResourceMesh->SetStaticMesh(ResourceData.BaseMesh[2]);
				break;
			}
			ROS_SetHarvestableArea();
		}
		else
		{
			Destroy();
		}
	}
}

void AResourceMasterBase::ROS_SetHarvestableArea_Implementation()
{
	FVector MinLocal, MaxLocal, NewExtentLocal;
	ResourceMesh->GetLocalBounds(MinLocal, MaxLocal);

	NewExtentLocal = FVector((FMath::Abs(MinLocal.X * CustomCollisionSize.X)), (FMath::Abs(MinLocal.Y * CustomCollisionSize.Y)), (FMath::Abs(MinLocal.Z * CustomCollisionSize.Z)));
	//FVector AdjusmentLocal = MaxLocal * CustomCollisionSize;

//	HarvestArea->SetBoxExtent(AdjusmentLocal, true);
	HarvestArea->SetBoxExtent(NewExtentLocal, true);

	if (bAdjustCollisionOffset)
	{
		HarvestArea->SetRelativeLocation(FVector(0.0f, 0.0f, NewExtentLocal.Z), false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		HarvestArea->SetRelativeLocation(ResourceMesh->GetRelativeLocation());
	}
}

void AResourceMasterBase::ROS_SetRowName_Implementation(FName Value)
{
	RowName = Value;
}
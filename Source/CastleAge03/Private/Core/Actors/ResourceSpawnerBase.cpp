// Code written by Zachary Parker for Feldfeste 


#include "Core/Actors/ResourceSpawnerBase.h"
#include "CoreMinimal.h"
#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#define CLAY SurfaceType1
#define DIRT SurfaceType2
#define SANDY SurfaceType3
#define ROCKY SurfaceType4
#define GRASS SurfaceType5

// Sets default values
AResourceSpawnerBase::AResourceSpawnerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Locate and load in the data table.*/
	ResourceDataTable = LoadObject<UDataTable>(NULL, TEXT("/Game/Core/DataTables/DT_Resources"));

	// Set this actor to replicate
	bReplicates = true;
	bAlwaysRelevant = true;

	// Set root component
	SpawningVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningVolume"));
	SetRootComponent(SpawningVolume);
	SpawningVolume->SetCollisionProfileName("NoCollision");
}

// Replicated variables
void AResourceSpawnerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AResourceSpawnerBase, SpawningVolume);
	DOREPLIFETIME(AResourceSpawnerBase, ResourceDataTable);
	DOREPLIFETIME(AResourceSpawnerBase, bSpawnerIsValid);
}

// Called when the game starts or when spawned
void AResourceSpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AResourceSpawnerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AResourceSpawnerBase::ROS_CheckSpawnerValidity_Implementation()
{
	// Prepare the variable used for checking data validity
	FlaggedNames.Empty();

	// Ensure the data table is loaded in.
	if (!ensure(ResourceDataTable != nullptr)) return;


	if (ResourceNames.IsValidIndex(0)) // Check if there are any entries to spawn TODO optimize. 
	{
		// Check if names are in the data table
		for (FName& TargetResource : ResourceNames) 
		{
			FResourceStruct* ResourceDataLocal = ResourceDataTable->FindRow<FResourceStruct>(TargetResource, "");
			bool ValidityCheckLocal = ResourceDataLocal ? true : false;
			if (!ValidityCheckLocal || !ResourceDataLocal->bProcedurally) // TODO Test
			{
				FlaggedNames.AddUnique(TargetResource); // if not add them to a list of names to be removed. 
			}
		}

		// Go through the flagged names and remove them from the spawn index
		for (FName& TargetRemove : FlaggedNames) 
		{
			int TargetIndex = -1;
			TargetIndex = ResourceNames.Find(TargetRemove);

			if (TargetIndex != -1)
			{
				ResourceNames.RemoveAt(TargetIndex);
#if WITH_EDITOR
				UE_LOG(LogTemp, Warning, TEXT("Name removed: %s at %d"), *TargetRemove.ToString(), TargetIndex);
#endif
			}	
		}

		// One final check if the array (after editing) is still valid. 
		if (ResourceNames.Last().IsValidIndexFast())
		{
			bSpawnerIsValid = true;
			return;
		}
		else
		{
			bSpawnerIsValid = false;
			return;
		}


	}
	else
	{
		bSpawnerIsValid = false;
		Destroy();
		return;
	}

}

void AResourceSpawnerBase::ROS_StartSpawner_Implementation(bool bIsValid)
{
	if (bIsValid)
	{
		for (FName& TargetResource : ResourceNames)
		{
			FResourceStruct* ResourceDataLocal = ResourceDataTable->FindRow<FResourceStruct>(TargetResource, "");
			bool ValidityCheckLocal = ResourceDataLocal ? true : false; // Technically not needed
			
			if (ValidityCheckLocal)
			{
				ROS_SpawnResource(TargetResource, *ResourceDataLocal);
			}
		}
	}
	else
	{
		if (IsValid(this))
			Destroy();
	}
}

void AResourceSpawnerBase::ROS_SpawnResource_Implementation(FName RowName, FResourceStruct ResourceDetails)
{
	for (size_t i = 0; i << ResourceDetails.Density; i++)
	{

		int CountCheckLocal{ 0 };
		bool bSpawned{ false };

		while(CountCheckLocal < 10 || !bSpawned)
		{
			FHitResult HitOut;
			FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(SpawningVolume->GetComponentLocation(), SpawningVolume->GetScaledBoxExtent());
			FCollisionQueryParams TraceParams(TEXT("LineOfSight_Trace"), false, this);

			if (GetWorld()->LineTraceSingleByChannel(HitOut, (RandomPoint+ (FVector(0.0f, 0.0f, 250.0f))), (RandomPoint - (FVector(0.0f, 0.0f, 250.0f))), ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam))
			{
				if (CheckAllowedSurfaceTypes(HitOut, ResourceDetails.AllowedSurfaces))
				{
					// set location
					FVector TranslationLocal = HitOut.ImpactPoint;
					// Set rotation
					FRotator RotationLocal = CalculateRotation(ResourceDetails.RotationX, ResourceDetails.RotationY, ResourceDetails.RotationZ);
					// Set Scale
					FVector ScaleLocal = CalculateScale(ResourceDetails.ScaleSettings, ResourceDetails.ScaleX, ResourceDetails.ScaleY, ResourceDetails.ScaleZ);

					FTransform ResourceTransform{ RotationLocal , TranslationLocal, ScaleLocal };
					AResourceMasterBase* ResourceLocal = Cast<AResourceMasterBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ResourceToSpawn, ResourceTransform));
					ResourceLocal->ROS_SetRowName(RowName);
					UGameplayStatics::FinishSpawningActor(ResourceLocal, ResourceTransform);
					bSpawned = true;
					return; // exit out of this function
				}
				else
				{
					CountCheckLocal++;
				}
			}
			else
			{
				CountCheckLocal++;
			}
		}
	}
}

bool AResourceSpawnerBase::CheckAllowedSurfaceTypes(FHitResult& Results, FAllowedSpawnLocations AllowedLocations)
{
	/*
	#define CLAY SurfaceType1
#define DIRT SurfaceType2
#define SANDY SurfaceType3
#define ROCKY SurfaceType4
#define GRASS SurfaceType5
	*/
	switch (UGameplayStatics::GetSurfaceType(Results))
	{
	case DIRT:
		if (AllowedLocations.bSpawnOnDirt)
			return true;
		else
			return false;
	case SANDY:
		if (AllowedLocations.bSpawnOnSandy)
			return true;
		else
			return false;
	case ROCKY:
		if (AllowedLocations.bSpawnOnRocky)
			return true;
		else
			return false;
	case GRASS:
		if (AllowedLocations.bSpawnOnGrass)
			return true;
		else
			return false;
	default:
		return false;
	}

}

FRotator AResourceSpawnerBase::CalculateRotation(FMInMaxStruct X, FMInMaxStruct Y, FMInMaxStruct Z)
{
	float RollLocal = FMath::RandRange(X.MinValue, X.MaxValue);
	float PitchLocal = FMath::RandRange(Y.MinValue, Y.MaxValue);
	float YawLocal = FMath::RandRange(Z.MinValue, Z.MaxValue);

	FRotator RotationLocal{ PitchLocal, YawLocal, RollLocal }; // WHY IS THIS Y Z X? COME ON EPIC!
	return RotationLocal;
}

FVector AResourceSpawnerBase::CalculateScale(TEnumAsByte <EScaleSettings> Setting, FMInMaxStruct X, FMInMaxStruct Y, FMInMaxStruct Z)
{
	FVector Result;
	float xValue, yValue, zValue;

	switch (Setting)
	{
	case Uniform:
		xValue = FMath::RandRange(X.MinValue, X.MaxValue);
		Result = FVector(xValue, xValue, xValue);
		return Result;
	case LockXY:
		xValue = FMath::RandRange(X.MinValue, X.MaxValue);
		zValue = FMath::RandRange(Z.MinValue, Z.MaxValue);
		Result = FVector(xValue, xValue, zValue);
		return Result;
	case Free:
		xValue = FMath::RandRange(X.MinValue, X.MaxValue);
		yValue = FMath::RandRange(Y.MinValue, Y.MaxValue);
		zValue = FMath::RandRange(Z.MinValue, Z.MaxValue);
		Result = FVector(xValue, yValue, zValue);
		return Result;
	default:
		xValue = FMath::RandRange(X.MinValue, X.MaxValue);
		yValue = FMath::RandRange(Y.MinValue, Y.MaxValue);
		zValue = FMath::RandRange(Z.MinValue, Z.MaxValue);
		Result = FVector(xValue, yValue, zValue);
		return Result;
	}
}
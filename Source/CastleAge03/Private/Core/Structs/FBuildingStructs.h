// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Core/Enums/ResourcesEnum.h"
#include "Core/Enums/BuildingEnums.h"
#include "FCommonStructs.h"
#include "FBuildingStructs.generated.h"

/************************************************
* Structures for buildings
************************************************/


/* This will be the STRUCT for the resource costs */
USTRUCT(BlueprintType)
struct FBuildingCosts : public FTableRowBase
{
	GENERATED_BODY()
	/* What is the max value*/
	UPROPERTY(EditAnywhere)
	float Amount;

	/* What is the min value*/
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EResources> Resource;
};

/* Primary structures for all buildings */
USTRUCT(BlueprintType)
struct FBuildingStruct : public FTableRowBase
{
	GENERATED_BODY()

	/* What category of building is this*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EBuildingType> BuildingType;
	/* What is the Building Name (default names only)*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EBuildingName> BuildingName;

	/* Building Health values */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMInMaxStruct Stability;
		
	/* The Collision Area Around the Building*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector GridSize;
	/* Height Bonus Related*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int HeightBonus;

	/* Is this a spline? Wall, roads, etc. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsSpline;
	/* Is this building made of stone? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsStoneBuilding;
	/* Does this need to be built on grass? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bMustBeOnGrass;
	/* Does this need to be built on a non-rock surface? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bMustNotBeOnRock;

	/* Does this connect to another building/Is it an add-on? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bSnaps;
	/* Checks if this can have addons*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanHaveAddOns;
	/* which can be added on to it? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TEnumAsByte<EBuildingName>> PossibleAddons;
	/* Checks if this can be a addon*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsAnAddon;
	/* which can be added on to it? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TEnumAsByte<EBuildingName>> PossibleBases;

	/* Array of Construction Stage Building Meshes */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UStaticMesh*> ConstructionMeshes;
	/* Array of Completed Building Meshes */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UStaticMesh*> BaseMeshes;
	/* Array of Damaged Building Meshes */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UStaticMesh*> DamageMeshes;
	/* Array of Burning Building Meshes */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UStaticMesh*> BurningMeshes;

	/* Construction Speed base */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BuildingTime;

	/* Costs of construction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FBuildingCosts> BuildingCosts;

	/* An Array (for languages) of Descriptions*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FText> Description;

	/* Occupants */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxWorkingOccupants;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxOccupants;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanHaveNonPeople;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxNonPeopleOccupants;

	/* Has a passive perk */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasPassivePerk;
	/* Has a special actions  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasSpecialActions;
	/* Checks if it has Torches */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasTorches;
};

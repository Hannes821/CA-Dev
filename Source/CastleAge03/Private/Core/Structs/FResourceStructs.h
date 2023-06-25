// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "Math/Color.h"
#include "Core/Enums/ResourcesEnum.h"
#include "Core/Enums/BuildingEnums.h"
#include "Core/Enums/CommonEnums.h"
#include "FCommonStructs.h"
#include "FResourceStructs.generated.h"

/************************************************
* Structures for resources
************************************************/

/* All resources */
USTRUCT(BlueprintType)
struct FResources : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct AnimalHide;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct BattleAxe;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Beer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Berries;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Bow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Bread;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Brick;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Ceramic;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Chainmail;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct ChalkStone;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Cheese;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Clay;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Cloth;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Coal;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Crossbow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Egg;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Fish;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Flax;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Flour;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Fruits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Gambeson;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Grain;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Honey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Hop;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Ice;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct IronIgnot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct IronOreBoulder;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct IronOrePowder;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Javelins;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Leather;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Meat;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct OrePowder;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Parchment;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Pennies;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Rope;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Saddle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Salt;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Sausages;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct SilverOreBoulder;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct SilverOrePowder;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct SlakedLime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Shield;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Spear;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Sword;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct TanningAgent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Tar;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct TreeBark;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Vegetables;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Vine;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Water;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct WoodBeam;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct WoodPlank;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Wool;

};
/* Base Raw Material Resources  */
/*/
USTRUCT(BlueprintType)
struct FRawResources : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Clay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Coal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Flax;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct IronOre;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Leather;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct StoneBrick;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Tar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct WoodBeam;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Wool;

};
*/

/* Managed Produced Resources  */
/*
USTRUCT(BlueprintType)
struct FProducedResources : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Bow;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Chainmail;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Cloth;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Crossbow;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Flax;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Gambeson;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Javelin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct OrePowder;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Saddle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Spear;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Sword;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct WoodShield;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct WoodPlank;
};
*/

/* Managed Food Resources  */
/*
USTRUCT(BlueprintType)
struct FRawFoodResources : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Beer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Bread;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Fish;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Flour;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Fruit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Grain;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Hop;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Sausage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Vegetables;

};
*/

/* Managed Food Resources  */
/*
USTRUCT(BlueprintType)
struct FProducedFoodResources : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Beer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Bread;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Flour;
};
*/


/* Managed VisualResources Resources  */
/*
USTRUCT(BlueprintType)
struct FVisualResources : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct AnimalHide;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct BattleAxe;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Beer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Berries;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Bow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Bread;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Brick;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Ceramic;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Chainmail;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct ChalkStone;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Cheese;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Clay;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Cloth;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Coal;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Crossbow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Egg;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Fish;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Flax;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Flour;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Fruits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Gambeson;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Grain;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Honey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Hop;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Ice;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct IronIgnot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct IronOreBoulder;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct IronOrePowder;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Javelins;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Leather;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Meat;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct OrePowder;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Parchment;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Pennies;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Rope;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Saddle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Salt;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Sausages;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct SilverOreBoulder;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct SilverOrePowder;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct SlakedLime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Shield;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Spear;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Sword;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct TanningAgent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Tar;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct TreeBark;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Vegetables;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Vine;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Water;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct WoodBeam;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct WoodPlank;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMaxCurrentStruct Wool;
};
*/

USTRUCT(BlueprintType)
struct FAllowedSpawnLocations : public FTableRowBase
{
	GENERATED_BODY()

//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//		bool bSpawnOnChalky{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bSpawnOnDirt{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bSpawnOnGrass{ false };
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//		bool bSpawnOnClay{ false };
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//		bool bSpawnOnLoamy{ false };
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//		bool bSpawnOnPeaty{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bSpawnOnRocky{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bSpawnOnSandy{ false };
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//		bool bSpawnOnSilty{ false };
};

USTRUCT(BlueprintType)
struct FResourceStruct : public FTableRowBase
{
	GENERATED_BODY()

	/* What is the deposit name*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName DepositName;
	/* what base resource type is it */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EResources> ResourceType;
	/* What type of mesh is displayed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EDepositeType> DepositType;

	/* Array of Basic/General Display Meshes (when not being harvested from) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UStaticMesh*> BaseMesh;
	/* Array of Basic/General Display Meshes when active harvesting has occured */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UStaticMesh*> HarvestingMesh;
	/* Array of meshes for resources being carried off from site */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UStaticMesh*> CarryMesh;
	/* Array of meshes for resources site has been attacked (e.g., trees burnt) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UStaticMesh*> DamageMesh;

	/* Does this resource respawn (only applies only to resources that are not infite) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRespawns{ false };
	/* How quickly (in seconds) does the resource respawn */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RespawnTimer{ 0.0 };
	/* Is this resource infite (only applies only to resources that do not respawn) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsInfinite{ false };
	
	/* Is this flagged to be procedurally genated (e.g., useful for rocks and trees) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bProcedurally{ false };
	/* What surfcaes is this allowed to procedurally spawn on */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAllowedSpawnLocations AllowedSurfaces;
	/* The density of resource when spawned in (more useful for trees and animals) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Density{0};
	/* The method for scalling  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte <EScaleSettings> ScaleSettings;
	/* The X scale of the mesh (used for procedural spawning) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMInMaxStruct ScaleX;
	/* The Y scale of the mesh (used for procedural spawning) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMInMaxStruct ScaleY;
	/* The Z scale of the mesh (used for procedural spawning) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMInMaxStruct ScaleZ;
	/* can this be randomly rotated when procedurally spawned */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRandomRotation{ true };
	/* The X Rotatin of the mesh (used for procedural spawning) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMInMaxStruct RotationX;
	/* The Y Rotatin of the mesh (used for procedural spawning) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMInMaxStruct RotationY;
	/* The Z Rotatin of the mesh (used for procedural spawning) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMInMaxStruct RotationZ;

	/* The icon for this resource */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* ResourceIcon;
	/* The overlay color resource */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor OverlayColor;
	/* Does this resource require a building to employ individuals in order to harvest? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRequiresEmploymentBuilding{ true };
	/* What, if any, building is needed to harvest this resource */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EBuildingName> EmploymentBuilding;
	/* What building(s) is needed to harvest this resource */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TEnumAsByte<EBuildingName>> StorageBuilding;

	/* If the resource can take damage (applies to non-infinite resources like trees) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaxCurrentStruct Stability;
};
// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"

/**
 * Enums for all resources
 */

UENUM(BlueprintType)
enum EResources
{
	AnimalHide UMETA(DisplayName = "Animal Hide"),
	BattleAxe UMETA(DisplayName = "Battle Axe"),
	Beer UMETA(DisplayName = "Beer"),
	Berries UMETA(DisplayName = "Berries"),
	Bow UMETA(DisplayName = "Bow"),
	Bread UMETA(DisplayName = "Bread"),
	Brick UMETA(DisplayName = "Brick"),
	Ceramic UMETA(DisplayName = "Ceramic"),
	Chainmail UMETA(DisplayName = "Chainmail"),
	ChalkStone UMETA(DisplayName = "Chalk Stone"),
	Cheese UMETA(DisplayName = "Cheese"),
	Clay UMETA(DisplayName = "Clay"),
	Cloth UMETA(DisplayName = "Cloth"),
	Coal UMETA(DisplayName = "Coal"),
	Crossbow UMETA(DisplayName = "Crossbow"),
	Egg UMETA(DisplayName = "Eggs"),
	Fish UMETA(DisplayName = "Fish"),
	Flax UMETA(DisplayName = "Flax"),
	Flour UMETA(DisplayName = "Flour"),
	Fruits UMETA(DisplayName = "Fruits"),
	Gambeson UMETA(DisplayName = "Gambeson"),
	Grain UMETA(DisplayName = "Grain"),
	Honey UMETA(DisplayName = "Honey"),
	Hop UMETA(DisplayName = "Hop"),
	Ice UMETA(DisplayName = "Ice"),
	IronIgnot UMETA(DisplayName = "Iron Ignot"),
	IronOreBoulder UMETA(DisplayName = "Iron Boulder"),
	IronOrePowder UMETA(DisplayName = "Iron Ore Powder"),
	Javelins UMETA(DisplayName = "Javelins"),
	Leather UMETA(DisplayName = "Leather"),
	Meat UMETA(DisplayName = "Meat"),
	OrePowder UMETA(DisplayName = "Ore Powder"),
	Parchment UMETA(DisplayName = "Parchment"),
	Pennies UMETA(DisplayName = "Pennies"),
	Rope UMETA(DisplayName = "Rope"),
	Saddle UMETA(DisplayName = "Saddle"),
	Salt UMETA(DisplayName = "Salt"),
	Sausages UMETA(DisplayName = "Sausages"),
	SilverOreBoulder UMETA(DisplayName = "Silver Ore Boulder"),
	SilverOrePowder UMETA(DisplayName = "Silver Ore Powder"),
	SlakedLime UMETA(DisplayName = "Slaked Lime"),
	Shield UMETA(DisplayName = "Shield"),
	Spear UMETA(DisplayName = "Spear"),
	Sword UMETA(DisplayName = "Sword"),
	TanningAgent UMETA(DisplayName = "Tanning Agent"),
	Tar UMETA(DisplayName = "Tar"),
	TreeBark UMETA(DisplayName = "Tree Bark"),
	Vegetables UMETA(DisplayName = "Vegetables"),
	Vine UMETA(DisplayName = "Vine"),
	Water UMETA(DisplayName = "Water"),
	WoodBeam UMETA(DisplayName = "Wood Beam"),
	WoodPlank UMETA(DisplayName = "Wood Plank"),
	Wool UMETA(DisplayName = "Wool")

};
/*
UENUM(BlueprintType)
enum EProducedResources 
{
	WoodBeam UMETA(DisplayName = "Wood Beam"),
	WoodPlank UMETA(DisplayName = "Wood Plank"),
	Javelin UMETA(DisplayName = "Javelin"),
	Bow UMETA(DisplayName = "Bow"),
	Crossbow UMETA(DisplayName = "Crossbow"),
	WoodenShield UMETA(DisplayName = "Wooden Shield"),
	Spear UMETA(DisplayName = "Spear"),
	IronOre UMETA(DisplayName = "Iron Ore"),
	OrePowder UMETA(DisplayName = "Ore Powder"),
	Chainmail UMETA(DisplayName = "Chainmail"),
	Sword UMETA(DisplayName = "Sword"),
	Clay UMETA(DisplayName = "Clay"),
	StoneBrick UMETA(DisplayName = "StoneBrick"),
	Tar UMETA(DisplayName = "Tar"),
	Flax UMETA(DisplayName = "Flax"),
	Wool UMETA(DisplayName = "Wool"),
	Leather UMETA(DisplayName = "Leather"),
	Cloth UMETA(DisplayName = "Cloth"),
	Gambeson UMETA(DisplayName = "Gambeson"),
	Saddle UMETA(DisplayName = "Saddle")
};

UENUM(BlueprintType)
enum EFoodResources
{
	Vegetables UMETA(DisplayName = "Vegetables"),
	Fish UMETA(DisplayName = "Fish"),
	Sausage UMETA(DisplayName = "Sausage"),
	Fruit UMETA(DisplayName = "Fruit"),
	Hop UMETA(DisplayName = "Hop"),
	Beer UMETA(DisplayName = "Beer"),
	Grain UMETA(DisplayName = "Grain"),
	Flour UMETA(DisplayName = "Flour"),
	Bread UMETA(DisplayName = "Bread")
};
*/
/*
UENUM(BlueprintType)
enum EVisualResources 
{
	Ice UMETA(DisplayName = "Ice"),
	CeramicWare UMETA(DisplayName = "Ceramic Ware"),
	Clothes UMETA(DisplayName = "Clothes"),
	Chicken UMETA(DisplayName = "Chicken"),
	Cat UMETA(DisplayName = "Cat"),
	Coinage UMETA(DisplayName = "Coinage"),
	SmithingTools UMETA(DisplayName = "Smithing Tools"),
	CarpentryTools UMETA(DisplayName = "Carpentry Tools"),
	Dog UMETA(DisplayName = "Dog"),
	Honey UMETA(DisplayName = "Honey"),
	Rope UMETA(DisplayName = "Rope"),
	SilverPence UMETA(DisplayName = "Silver Pence"),
	Parchment UMETA(DisplayName = "Parchment")
};
*/









UENUM(BlueprintType)
enum EDepositeType
{
	Animal UMETA(DisplayName = "Animal"),
	Building UMETA(DisplayName = "Building"),
	Cliff UMETA(DisplayName = "Cliff"),
	Tree UMETA(DisplayName = "Tree")
};
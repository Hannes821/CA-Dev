// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"

/**
 * Enums for all resources
 */

UENUM(BlueprintType)
enum EPHysicalSurfaceTypes
{
	Chalky UMETA(DisplayName = "Chalky"),
	ClaySurface UMETA(DisplayName = "Clay"),
	Loamy UMETA(DisplayName = "Loamy"),
	Peaty UMETA(DisplayName = "Peaty"),
	Rocky UMETA(DisplayName = "Rocky"),
	Sandy UMETA(DisplayName = "Sandy"),
	Silty UMETA(DisplayName = "Silty")
};

UENUM(BlueprintType)
enum EScaleSettings
{
	Uniform UMETA(DisplayName = "Uniform"),
	LockXY UMETA(DisplayName = "Lock XY"),
	Free UMETA(DisplayName = "Free")
};

UENUM(BlueprintType)
enum ESeasons
{
	SeasonWinter UMETA(DisplayName = "Winter"),
	SeasonSpring UMETA(DisplayName = "Spring"),
	SeasonSummer UMETA(DisplayName = "Summer"),
	SeasonFall UMETA(DisplayName = "Fall")
};
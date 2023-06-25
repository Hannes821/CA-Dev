// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"

/**
 * Enum for player input modes.
 * Made global so that other classes can pass data if needed
 */

UENUM(BlueprintType)
enum EPrimaryActionModes
{
	Selection UMETA(DisplayName = "Select Mode"), // this is default as well.
	Construction UMETA(DisplayName = "Construction Mode"), // for moving previews and setting construction sites
	Deletion UMETA(DisplayName = "Delete Mode"), // for deleting construction sites
	Disabled UMETA(DisplayName = "Disabled") // to disable input. Please remember to enable.
};

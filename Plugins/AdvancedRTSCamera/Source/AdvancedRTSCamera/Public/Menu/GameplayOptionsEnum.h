// Copyright Two Neurons Studio, LLC. Year of Publication 2024.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EGameplaySettings
{
	GS_MovementSpeed = 0			UMETA(DisplayName = "Movement Speed"),
	GS_PanSensitivity				UMETA(DisplayName = "Pan Sensitivity")
};
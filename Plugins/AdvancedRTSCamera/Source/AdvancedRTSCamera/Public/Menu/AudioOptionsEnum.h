// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AudioOptionsEnum.generated.h"

/**
 * Enum or various AUDIO scalability settings
 */
UENUM(BlueprintType)
enum EAudioSettingsOptions 
{
    AS_Master = 0             UMETA(DisplayName = "Master"),
    AS_Ambient                UMETA(DisplayName = "Ambient"),
    AS_Dialogue               UMETA(DisplayName = "Dialogue"),
    AS_Effects                UMETA(DisplayName = "Effects"),
    AS_Music                  UMETA(DisplayName = "Music"),
};
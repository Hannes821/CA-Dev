// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ScalabilityOptionsEnum.generated.h"

/**
 * Enums for various VISUAL scalability settings
 */
UENUM(BlueprintType)
enum EVisualQualityOptions
{
    Overall = 0             UMETA(DisplayName = "Overall"),
    AntiAliasing            UMETA(DisplayName = "Anti-Aliasing"),
    Foliage                 UMETA(DisplayName = "Foliage"),
    GlobalIllumination      UMETA(DisplayName = "Global Illumination"),
    PostProcessing          UMETA(DisplayName = "Post Processing"),
    Reflections             UMETA(DisplayName = "Reflections"),
    Shading                 UMETA(DisplayName = "Shading"),
    Shadows                 UMETA(DisplayName = "Shadows"),
    Textures                UMETA(DisplayName = "Textures"),
    ViewDistance            UMETA(DisplayName = "View Distance"),
    VisualEffects           UMETA(DisplayName = "Visual Effects"),

};

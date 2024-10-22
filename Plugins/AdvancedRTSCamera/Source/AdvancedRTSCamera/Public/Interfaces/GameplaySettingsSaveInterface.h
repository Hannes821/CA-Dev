// Copyright Two Neurons Studio, LLC. Year of Publication 2024.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Menu/GameplayOptionsEnum.h"
#include "GameplaySettingsSaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameplaySettingsSaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCEDRTSCAMERA_API IGameplaySettingsSaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/* Change the Gameplay setting for a particular audio type (e.g., dialogue, ambient) to a new value */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameplaySetting")
	bool TriggerAdjustGameplaySettings(EGameplaySettings Key, float NewValueIn);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameplaySetting")
	bool ForceSaveGameplaySettings();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameplaySetting")
	void TriggerLoadGameplaySettings();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameplaySetting")
	TMap<TEnumAsByte<EGameplaySettings>, float> TriggerGetGameplaySettings();
};

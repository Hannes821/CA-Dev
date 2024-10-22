// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Systems/SettingsSaveMaster.h"
#include "AudioSaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAudioSaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface handles communication to/from the audio settings widget and the save system
 */
class ADVANCEDRTSCAMERA_API IAudioSaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AudioSettings")
	void TriggerAdjustAudioSetting(EAudioSettingsOptions Key, float NewValueIn);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AudioSettings")
	void TriggerLoadAudioSettings();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AudioSettings")
	TMap<TEnumAsByte<EAudioSettingsOptions>, float> TriggerGetAudioSettings();
};

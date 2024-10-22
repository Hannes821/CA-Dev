// Copyright Two Neurons Studio, LLC. Year of Publication 2024.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "SettingsSaveMaster.h"
#include "Interfaces/InputSaveInterface.h"
#include "Interfaces/AudioSaveInterface.h"
#include "Interfaces/GameplaySettingsSaveInterface.h"
#include "RTSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDRTSCAMERA_API URTSGameInstance : public UGameInstance, public IInputSaveInterface, public IAudioSaveInterface, public IGameplaySettingsSaveInterface
{
	GENERATED_BODY()

/************************************************
* ATTRIBUTES
************************************************/
protected:
	FString GameSettings { "SettingsSlot" };
	UPROPERTY(BlueprintReadOnly, Category = "SaveSystem")
	USettingsSaveMaster* SettingSave;

	UPROPERTY(EditDefaultsOnly, Category = "AudioSettings")
	class USoundMix* MasterMix;
	UPROPERTY(EditDefaultsOnly, Category = "AudioSettings")
	class USoundClass* AmbientSoundClass;
	UPROPERTY(EditDefaultsOnly, Category = "AudioSettings")
	class USoundClass* DialogueSoundClass;
	UPROPERTY(EditDefaultsOnly, Category = "AudioSettings")
	class USoundClass* EffectSoundClass;
	UPROPERTY(EditDefaultsOnly, Category = "AudioSettings")
	class USoundClass* MusicSoundClass;

/************************************************
* METHODS
************************************************/
	/************************************************
	* INHEREITED METHODS
	************************************************/
protected:
	void Init() override;
	void Shutdown() override;


	/************************************************
	* MEMBER METHODS
	************************************************/
	UFUNCTION(BlueprintCallable, Category = "SettingsMaster")
	void SaveSettings();


	UFUNCTION(BlueprintCallable, Category = "Keybindings")
	void LoadKeybindings();
	UFUNCTION(BlueprintCallable, Category = "Keybindings")
	void CreateSettingsSave();

	void LoadSaveData();



	UFUNCTION(BlueprintCallable, Category = "AudioSettings")
	void LoadAudioSettings();

	UFUNCTION(BlueprintCallable, Category = "GameplaySettings")
	void LoadGameplaySettings();

	/************************************************
	* INTERFACE EVENTS FOR KEYBINDINGS
	************************************************/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "General")
	USettingsSaveMaster* GetSavedSettings();
	USettingsSaveMaster* GetSavedSettings_Implementation();

	/* Used to call load bindings from other classes than the class that inherits from this interface*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Keybinding|Saves")
	void TriggerLoadBindings();
	void TriggerLoadBindings_Implementation();
	/* Used to call save bindings from other classes than the class that inherits from this interface*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Keybinding|Saves")
	void TriggerSaveBindings();
	void TriggerSaveBindings_Implementation();
	/** Use to update existing custom key bind */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Keybinding|Saves")
	void UpdateKeyBindings(const FName& Name, const FKey& Key, int Index);
	void UpdateKeyBindings_Implementation(const FName& Name, const FKey& Key, int Index);
	/* Used to add new custom key bind */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Keybinding|Saves")
	void AddKeyBinding(const FName& Name, const FKey& Key);
	void AddKeyBinding_Implementation(const FName& Name, const FKey& Key);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Keybinding|Saves")
	void RemoveBindingByIndex(int Index);
	void RemoveBindingByIndex_Implementation(int Index);

	/************************************************
	* INTERFACE EVENTS FOR AUDIO SETTINGS
	************************************************/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AudioSettings")
	bool TriggerAdjustAudioSettings(EAudioSettingsOptions Key, float NewValueIn);
	bool TriggerAdjustAudioSettings_Implementation(EAudioSettingsOptions Key, float NewValueIn);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AudioSettings")
	bool ForceSaveAudioSettings();
	bool ForceSaveAudioSettings_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AudioSettings")
	TMap<TEnumAsByte<EAudioSettingsOptions>, float> TriggerGetAudioSettings();
	TMap<TEnumAsByte<EAudioSettingsOptions>, float> TriggerGetAudioSettings_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AudioSettings")
	void TriggerLoadAudioSettings();
	void TriggerLoadAudioSettings_Implementation();

	/************************************************
	* INTERFACE EVENTS FOR GAMEPLAY SETTINGS
	************************************************/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameplaySetting")
	bool TriggerAdjustGameplaySettings(EGameplaySettings Key, float NewValueIn);
	bool TriggerAdjustGameplaySettings_Implementation(EGameplaySettings Key, float NewValueIn);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameplaySetting")
	bool ForceSaveGameplaySettings();
	bool ForceSaveGameplaySettings_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameplaySetting")
	void TriggerLoadGameplaySettings();
	void TriggerLoadGameplaySettings_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameplaySetting")
	TMap<TEnumAsByte<EGameplaySettings>, float> TriggerGetGameplaySettings();
	TMap<TEnumAsByte<EGameplaySettings>, float> TriggerGetGameplaySettings_Implementation();
};
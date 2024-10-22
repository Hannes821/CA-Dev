// Copywritten by Two Neurons Studio, LLC. All rights reserved. Year of Publication 2024.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Menu/KeyMappingStructs.h"
#include "Menu/AudioOptionsEnum.h"
#include "Menu/GameplayOptionsEnum.h"
#include "SettingsSaveMaster.generated.h"

/**
 * This class is the saved game object for the settings. 
 * This class contains structs for game bindings and other relevant (to settings) saves.
 * These structs are then declared 
 */
UCLASS()
class ADVANCEDRTSCAMERA_API USettingsSaveMaster : public USaveGame
{
	GENERATED_BODY()

private:
	/** The save struct for the keybindings */
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Keybindings", meta = (AllowPrivateAccess = "true"))
	FKeyBindingsSave KeyBindingsSave;

	/** The audio settings save. The default is editable to make this more easily extendable in blueprint */
	UPROPERTY(SaveGame, BlueprintReadOnly, EditDefaultsOnly, Category = "AudioSettings", meta = (AllowPrivateAccess = "true"))
	TMap<TEnumAsByte<EAudioSettingsOptions>, float> AudioSettingsSave;
	/* The gameplay setting save. */
	UPROPERTY(SaveGame, BlueprintReadOnly, EditDefaultsOnly, Category = "GameplaySettings", meta = (AllowPrivateAccess = "true"))
	TMap<TEnumAsByte<EGameplaySettings>, float> GameplaySettingsSave;


	/**************************************
	* General
	**************************************/
	USettingsSaveMaster(const FObjectInitializer& ObjectInitializer);

	/**************************************
	* KEYBINDINGS (KEPS AND MAPS)
	**************************************/
	// Keys
public:
	void SetKeyOverride(const FKey& Key, size_t Index);

	void AddKeyOverride(const FKey& Key);

	void RemoveKeyOverrideAt(size_t Index);

	void RemoveAllkeyOverrides();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Keybindings|Keys")
	FKeyBindingsSave GetKeybindingsSave() const { return KeyBindingsSave; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Keybindings|Keys")
	TArray<FKey> GetKeyOverrides();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Keybindings|Keys")
	int FindKeyOverride(const FKey& Key);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Keybindings|Keys")
	FKey GetKeyOverride(int Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Keybindings|Keys")
	bool IsKeyOverrideValid(const FKey& Key);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Keybindings|Keys")
	bool IsKeyIndexValid(int Index);


// Mapping Names
	void SetMappingName(const FName& Name, size_t Index);

	void AddMappingName(const FName& Name);

	void RemoveMappingNameAt(size_t Index);

	void RemoveAllMappingNames();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Keybindings|MappingNames")
	TArray<FName> GetMappingNames();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Keybindings|MappingNames")
	int FindMappingName(const FName& Name);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Keybindings|MappingNames")
	FName GetMappingName(int Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Keybindings|MappingNames")
	bool IsMappingNameValid(const FName& Name);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Keybindings|MappingNames")
	bool IsMappingNameIndexValid(int Index);

	/**************************************
	* AUDIO SETTINGS
	**************************************/
	// TMap<FString, float> AudioSettingsSave
	void AdjustAudioSetting(EAudioSettingsOptions Key, float NewValueIn);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AudioSettings")
	TMap<TEnumAsByte<EAudioSettingsOptions>, float> GetAudioSettings() const { return AudioSettingsSave; }

	/**************************************
	* MEMBER METHODS - GAMEPLAY SETTINGS
	**************************************/
	void AdjustGameplaySetting(EGameplaySettings Key, float NewValueIn);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AudioSettings")
	TMap<TEnumAsByte<EGameplaySettings>, float> GetGameplaySettings() const { return GameplaySettingsSave; }
};

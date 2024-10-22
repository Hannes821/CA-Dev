// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "OptionsMenuMaster.h"
#include "IndividualAudioSelector.h"
#include "AudioSettingMenu.generated.h"

/**
 * This widget allows the user to adjust audio settings. It saves the settings when the player selects "accept"
 */
UCLASS()
class ADVANCEDRTSCAMERA_API UAudioSettingMenu : public UOptionsMenuMaster
{
	GENERATED_BODY()

	friend class UIndividualAudioSelector;

/************************************************
* BOUND ATTRIBUTES
************************************************/
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualAudioSelector* MasterSetting;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualAudioSelector* AmbientSetting;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualAudioSelector* DialogueSetting;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualAudioSelector* EffectsSetting;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualAudioSelector* MusicSetting;
/************************************************
* REFERENCE ATTRIBUTES
************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UGameInstance* GameInst;

/************************************************
* VARIABLE ATTRIBUTES
************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TArray<UIndividualAudioSelector*> AudioSelectors;

	UPROPERTY(BlueprintReadOnly, Category = "Targets")
	float MasterVolumeSetting{ 1.f };

	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AudioSettings")
	class USoundMix* MasterMix;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AudioSettings")
	class USoundClass* AmbientSoundClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AudioSettings")
	class USoundClass* DialogueSoundClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AudioSettings")
	class USoundClass* EffectSoundClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AudioSettings")
	class USoundClass* MusicSoundClass;
/************************************************
* METHODS
************************************************/
protected:
	bool Initialize() override;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void CreateSelectorArray();
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void LoadAudioSettings();

	void OnChangeDetected();

	void OnAccept_Implementation() override;
	void OnDefault_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Event")
	void SetMasterVolume();

	UFUNCTION(BlueprintCallable, Category = "Event")
	void ApplyAudioSettings();
};

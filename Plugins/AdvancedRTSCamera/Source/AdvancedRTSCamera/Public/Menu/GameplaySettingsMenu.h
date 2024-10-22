// Copyright Two Neurons Studio, LLC. Year of Publication 2024.

#pragma once

#include "CoreMinimal.h"
#include "Menu/OptionsMenuMaster.h"
#include "Menu/IndividualGamePlaySetting.h"
#include "Camera/MovementSettingsStructs.h"
#include "GameplaySettingsMenu.generated.h"

/**
 * This widget allows the user to adjust the gameplay settings as defined by a developer
 * Settings are loaded by the local player NOT by the widget
 */
UCLASS()
class ADVANCEDRTSCAMERA_API UGameplaySettingsMenu : public UOptionsMenuMaster
{
	GENERATED_BODY()
	
	// This friend stores this widget
	friend class UPauseMenu;
	// This friend is added by developers (not spawned) as part of the WBP derived from this class. There is communication between these two classes
	friend class UIndividualGamePlaySetting;

/************************************************
* ATTRIBUTES
************************************************/
	/************************************************
	* REFERENCE ATTRIBUTES
	************************************************/
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Referencs")
	class UGameInstance* GameInst;

	/************************************************
	* VARIABLE ATTRIBUTES
	************************************************/
	/* An array of the individual selectors */
	UPROPERTY(BlueprintReadWrite, Category = "Components")
	TArray<UIndividualGamePlaySetting*> GameplaySettingSelectors;
	UPROPERTY(BlueprintReadOnly, Category = "Targets")
	TArray<FMovementSettings> StoredSettings;
	TArray<float> GameplayDefaults;
/************************************************
* METHODS
************************************************/
	/************************************************
	* INHERITED METHODS
	************************************************/
protected:
	bool Initialize() override;

	virtual void NativeConstruct() override;

	void OnCancel_Implementation() override;
	void OnAccept_Implementation() override;
	void OnDefault_Implementation() override;

	/************************************************
	* MEMBER METHODS
	************************************************/
	/* Defined in derived WBP */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SetUp")
	void CreateSelectorArray();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SetUp")
	void InitSettings(const TArray<FMovementSettings>& SettingsIn, TArray<float>& GameplayDefaultsIn);
	void InitSettings_Implementation(const TArray<FMovementSettings>& SettingsIn, TArray<float>& GameplayDefaultsIn);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SetUp")
	void PassSettings(UIndividualGamePlaySetting* Selector, FMovementSettings SettingsIn);
	void PassSettings_Implementation(UIndividualGamePlaySetting* Selector, FMovementSettings SettingsIn);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SetUp")
	void LoadGameplaySettings();
	void LoadGameplaySettings_Implementation();

	void OnChangesDetected();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Event")
	void ApplyGameplaySettings();
	void ApplyGameplaySettings_Implementation();
};

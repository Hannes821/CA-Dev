// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "OptionsMenuMaster.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "VisualSettingSelector.h"
#include "VisualSettingsMenu.generated.h"


/**
 * This is the menu that allows the user to change the game visual scallability options
 */

UENUM(BlueprintType)
enum EScallabilityWindowMode
{
	FullscreenWM = 0					UMETA(DisplayName = "Fullscreen"),
	WindowedFM							UMETA(DisplayName = "Windowed"),
	WindowedFullScreenFM                UMETA(DisplayName = "Windowed Fullscreen")
};


UCLASS()
class ADVANCEDRTSCAMERA_API UVisualSettingsMenu : public UOptionsMenuMaster
{
	GENERATED_BODY()

	friend class UIndividualSetting;


/************************************************
* ATTRIBUTES
************************************************/
private:
	const int LOW{ 0 };
	const int MEDIUM{ 1 };
	const int HIGH{ 2 };
	const int EPIC{ 3 };
	const int CUSTOM{ 4 };
	const FString RES{ "X" };

protected:
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/
	/** The anchor that holds the scrollbox and the buttons. 
	This box is disabled and enabled at certain points. */
//	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
//	class UVerticalBox* SettingsVerticalAnchor; // TODO remove
	/** The scroll box for scrolling through the options. */
//	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
//	class UScrollBox* SettingScroll;
	/** List the available screen modes to the user */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UComboBoxString* ScreenModeOptions;
	/** Lists available resolutions to the user */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UComboBoxString* ResolutionOptions;
	/** checkbox for VSync */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UCheckBox* VSyncCheckBox;
	/** Setting widget for overall scalability settings */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualSetting* OverallQualitySelector;
	/** Setting widget for anti-aliasing scalability settings */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualSetting* AntiAliasingSelector;
	/** Setting widget for foliage scalability settings */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualSetting* FoliageSelector;
	/** Setting widget for global illumination (lumen) scalability settings */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualSetting* GlobalIlluminationSelector;
	/** Setting widget for Post-Processing scalability settings */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualSetting* PostProcessingSelector;
	/** Setting widget for Reflections scalability settings */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualSetting* ReflectionsSelector;
	/** Setting widget for Shading scalability settings */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualSetting* ShadingSelector;
	/** Setting widget for Shadows illumination (lumen) scalability settings */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualSetting* ShadowsSelector;
	/** Setting widget for Textures scalability settings */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualSetting* TextureSelector;
	/** Setting widget for View Distance scalability settings */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualSetting* ViewDistanceSelector;
	/** Setting widget for Visual Effects scalability settings */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UIndividualSetting* VisualEffectsSelector;

	/************************************************
	* VARIABLE ATTRIBUTES
	************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Settings")
	class UGameUserSettings* Settings;

//	UPROPERTY(BlueprintReadWrite, Category = "Status")
//	bool bChangeDetected{ false }; // todo remove

	/** Allows for fullscreen options */
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	bool bAllowFullscreen{ true };
	/** Allows for windowed fullscreen/borderless fullscreen */
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	bool bAllowWindowedFullScreen{ true };
	/** Allows for windowed - should be true if the above is true */
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	bool bAllowWindowed{ true };
	/** An array of the visual setting options (text + selector) */
	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	TArray<UIndividualSetting*> VisualSettingOptions;
	/** AN array for the selectors being used by the VisualSettingOptions */
	//UPROPERTY(BlueprintReadWrite, Category = "Settings")
	//TArray<UVisualSettingSelector*> VideoSettings;
	/** Used for a switch statement only (to avoid string use) */
	TEnumAsByte<EScallabilityWindowMode> SelectedWindowMode{ EScallabilityWindowMode::FullscreenWM };
/************************************************
* METHODS
************************************************/
protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void InitVideoSettings();
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void FullyInitVideoSettings();
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void SetScreenTypeOptions();
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void SetResolutionOptions();
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void GetDefaultVideoSettings();

	UFUNCTION(BlueprintCallable, Category = "Updates")
	void UpdateValues(bool Bypass = false, int Index = 0);
	UFUNCTION(BlueprintCallable, Category = "Updates")
	void ApplyVideoSettings();
	UFUNCTION(BlueprintCallable, Category = "Updates")
	void SetVSync();
	UFUNCTION(BlueprintCallable, Category = "Updates")
	void SetWindowedMode();
	UFUNCTION(BlueprintCallable, Category = "Updates")
	void SetScreenResolution();
	UFUNCTION(BlueprintCallable, Category = "Updates")
	void SetScallabilityOptions();

	void OnAccept_Implementation() override;
	void OnDefault_Implementation() override;
	void ForceClose_Implementation() override;
	void ResumeEditing_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Event")
	void OnUpdateScreenMode(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void OnUpdateResolution(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void OnUpdateVSync(bool IsChecked);

};

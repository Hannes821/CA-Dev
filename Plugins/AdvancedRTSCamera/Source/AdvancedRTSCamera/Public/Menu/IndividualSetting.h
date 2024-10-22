// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ScalabilityOptionsEnum.h"
#include "VisualSettingSelector.h"
#include "IndividualSetting.generated.h"

/**
 * The widget that list the area of visual scallability and the selector
 */
UCLASS()
class ADVANCEDRTSCAMERA_API UIndividualSetting : public UUserWidget
{
	GENERATED_BODY()


	friend class UVisualSettingSelector;
	friend class UVisualSettingsMenu;

/************************************************
* ATTRIBUTES
************************************************/
protected:
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/	
	/** The displayed name for the setting being adjusted */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UTextBlock* SettingItemText;
	/** The selector for this setting */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UVisualSettingSelector* SettingItemOption;

	/************************************************
	* REFERENCE ATTRIBUTES
	************************************************/
	/** Reference to the visual scalability settings menu */
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UVisualSettingsMenu* SettingsMenu;

	/************************************************
	* VARIABLE ATTRIBUTES
	************************************************/
	/** An index used determining which setting to update in the settings menu itself */
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Value")
	int Index{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Value")
	TEnumAsByte<EVisualQualityOptions> SettingTarget{ EVisualQualityOptions::Overall };

/************************************************
* METHODS
************************************************/
	virtual void NativeConstruct() override;

	/** Method to pass info from selector to settings menu through this widget*/
	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnUpdate();

	/** Stores widget that contains this individual setting */
	UFUNCTION(BlueprintCallable, Category = "References")
	void SetParentWidget(UVisualSettingsMenu* ParentWidget);

	/** Sets the index for this widget (index is used by settings menu to determine which settings are updated) */
	void SetIndex(int NewIndex);

	/** Used to directly (instead of incrementing) value settings from the settings widget to the selector*/
	UFUNCTION(BlueprintCallable, Category = "Events")
	void SetValue(int SettingValue);

	/************************************************
	* METHODS - GETTERS
	************************************************/
	/** Returns the selector widget */
	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters|Components")
	//FORCEINLINE class UVisualSettingSelector* GetSettingItemOption() const { return SettingItemOption; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters|Value")
	FORCEINLINE int GetValue() const { return SettingItemOption->GetSettingValue(); }

};

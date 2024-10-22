// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "VisualSettingSelector.generated.h"

/**
 * The widget for the setting selector.
 * This only lists the visual scalability
 */

UCLASS()
class ADVANCEDRTSCAMERA_API UVisualSettingSelector : public UUserWidget
{
	GENERATED_BODY()

	friend class UIndividualSetting;

/************************************************
* ATTRIBUTES
************************************************/
protected:
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* DownButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* UpButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UTextBlock* SettingText;

	/************************************************
	* REFERENCE ATTRIBUTES
	************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UIndividualSetting* IndividualSetting;

	/************************************************
	* VARIABLE ATTRIBUTES
	************************************************/
	/* The value of the scalability is at currently (FOR DISPLAY ONLY) */
	UPROPERTY(BlueprintReadWrite, Category = "Settings|Value")
	int SettingValue{ 0 };
	/* The value that previously was used  */
	UPROPERTY(BlueprintReadWrite, Category = "Settings|Value")
	int PreviousValue{ 0 };

	/* The key (name) of the scalability */
	UPROPERTY(BlueprintReadWrite, Category = "Settings|Key")
	FText ValueText{ FText::FromString("Overall") };

/************************************************
* METHODS
************************************************/
protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnDownButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnUpButtonClicked();

public:
	/** Stores widget that contains this selector */
	UFUNCTION(BlueprintCallable, Category = "References")
	void SetItemParentWidget(UIndividualSetting* ParentWidget);

	/** Changes the value of the setting and calls SetValueText, which updates the key value visually ON BUTTON CLICK */
	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnChange(int ChangeBy);

	/* Changes the value directly and calls SetValueText, used when settings widget updates the individual settings (NOT button click) */
	UFUNCTION(BlueprintCallable, Category = "EventCall")
	void SetValue(int SettingValueIn);

	/** Visually updates key value */
	UFUNCTION(BlueprintCallable, Category = "Events")
	void SetValueText();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters|Value")
	FORCEINLINE int GetSettingValue() const { return SettingValue; }
};

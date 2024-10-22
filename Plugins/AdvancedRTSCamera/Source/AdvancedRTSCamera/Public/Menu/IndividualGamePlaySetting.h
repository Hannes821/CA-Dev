// Copyright Two Neurons Studio, LLC. Year of Publication 2024.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Textblock.h"
#include "Components/Slider.h"
#include "Camera/MovementSettingsStructs.h"
#include "Menu/GameplayOptionsEnum.h"
#include "IndividualGamePlaySetting.generated.h"

/**
 * This is 'sub' widget that lists a gameplay setting (typically with a slider).
 * These have to be manually added to the GamePlaySettingsMenu in a WBP.
 * It will contain title (text), a slider, and percentage value (text)
 * Settings are loaded by the local player NOT by the widget
 */
UCLASS()
class ADVANCEDRTSCAMERA_API UIndividualGamePlaySetting : public UUserWidget
{
	GENERATED_BODY()

	// friend class that houses this widget. Communcation will occur between the two classes.
	friend class UGameplaySettingsMenu;

	/************************************************
* ATTRIBUTES
************************************************/
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UTextBlock* TargetGameplaySettingText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class USlider* GameplaySettingSlider;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UTextBlock* PercentageText;

	/************************************************
	* REFERENCE ATTRIBUTES
	************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UGameplaySettingsMenu* ParentWidget;

	/************************************************
	* VARIABLE ATTRIBUTES
	************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Targets")
	float SettingsValue{ 1.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Targets")
	FText SettingsText = FText::FromString(TEXT("Null"));
	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Targets")
	TEnumAsByte<EGameplaySettings> SettingTarget;
	UPROPERTY(BlueprintReadOnly, Category = "Targets")
	FMovementSettings TargetSetting{ 0.f, -1.f, 0.f };
	
/************************************************
* METHODS
************************************************/
	/************************************************
	* INHERITED METHODS
	************************************************/
protected:
	bool Initialize() override;

	void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	/************************************************
	* MEMBER METHODS
	************************************************/
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void SetTargetGameplaySettingText();
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetParentWidget(UGameplaySettingsMenu* ParentWidgetIn);
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetTargetSetting(FMovementSettings SettingsIn);
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void InitSlider(float InValue);

	UFUNCTION(BlueprintCallable, Category = "Events")
	void UpdateSlider(float InValue);
	UFUNCTION(BlueprintCallable, Category = "Events")
	void SetTargetValue(float InValue);
	UFUNCTION(BlueprintCallable, Category = "Events")
	void SetPercentageText(float InValue);


	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "MathHelpers")
	FORCEINLINE float GetRatio(float Value, float Minimum, float Maximum) const
	{
		if (Value > Maximum)
			return 1.0f;
		else if (Value > Minimum)
			return (Value - Minimum) / (Maximum - Minimum);
		else
			return Minimum;
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "MathHelpers")
		FORCEINLINE float GetPercentage(float Value, float Minimum, float Maximum) const
	{
		return GetRatio(Value, Minimum, Maximum) * 100;
	}
};

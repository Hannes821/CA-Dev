// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "AudioOptionsEnum.h"
#include "IndividualAudioSelector.generated.h"

/**
 * This widget list a sound class volume settings
 */
UCLASS()
class ADVANCEDRTSCAMERA_API UIndividualAudioSelector : public UUserWidget
{
	GENERATED_BODY()

friend class UAudioSettingMenu;

/************************************************
* ATTRIBUTES
************************************************/
protected:
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UTextBlock* TargetAudioText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class USlider* AudioSettingSlider;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UTextBlock* PercentageText;

	/************************************************
	* REFERENCE ATTRIBUTES
	************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UAudioSettingMenu* ParentWidget;

	/************************************************
	* VARIABLE ATTRIBUTES
	************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Targets")
	float SettingsValue{ 1.0f };

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Targets")
	TEnumAsByte<EAudioSettingsOptions> AudioTarget;

/************************************************
* METHODS
************************************************/
protected:
	bool Initialize() override;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void SetTargetAudioText();
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void SetParentWidget(UAudioSettingMenu* ParentWidgetIn);

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

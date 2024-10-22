// Copyright Two Neurons Studio, LLC. Year of Publication 2024.


#include "Menu/IndividualGamePlaySetting.h"
#include "Menu/GameplaySettingsMenu.h"
#include "Kismet/KismetTextLibrary.h"

bool UIndividualGamePlaySetting::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	// Create binding for slider (after clearing any previous settings)
	GameplaySettingSlider->OnValueChanged.Clear();
	GameplaySettingSlider->OnValueChanged.AddUniqueDynamic(this, &UIndividualGamePlaySetting::UpdateSlider);

	return true;
}

void UIndividualGamePlaySetting::NativePreConstruct()
{
	SetTargetGameplaySettingText();
}

void UIndividualGamePlaySetting::NativeConstruct()
{
	// load values form player
	if(TargetSetting.IsValid())
	{
		InitSlider(TargetSetting.Current);
		SettingsValue = GameplaySettingSlider->GetValue();
	}
	SetPercentageText(SettingsValue);
}

void UIndividualGamePlaySetting::SetTargetGameplaySettingText()
{
	// uncomment the following line and comment out the switch, if you want to use a FText to set the value
	// TargetGameplaySettingText->SetText(SettingsText); 
	switch (SettingTarget)
	{
	case GS_MovementSpeed:
		TargetGameplaySettingText->SetText(FText::FromString("Movement Speed"));
		break;
	case GS_PanSensitivity:
		TargetGameplaySettingText->SetText(FText::FromString("Pan Sensitivity"));
		break;
	default:
		TargetGameplaySettingText->SetText(FText::FromString("Error!"));
		break;
	}
}

void UIndividualGamePlaySetting::SetParentWidget(UGameplaySettingsMenu* ParentWidgetIn)
{
	ParentWidget = ParentWidgetIn;
}

void UIndividualGamePlaySetting::SetTargetSetting(FMovementSettings SettingsIn)
{
	if(SettingsIn.IsValid())
		TargetSetting = SettingsIn;
}

void UIndividualGamePlaySetting::InitSlider(float InValue)
{
	float NormalizedValue = FMath::GetMappedRangeValueClamped(FVector2D(TargetSetting.Min, TargetSetting.Max), FVector2D(0.f, 1.f), InValue);
	SetTargetValue(NormalizedValue);
	SetPercentageText(GameplaySettingSlider->GetValue());
}

void UIndividualGamePlaySetting::UpdateSlider(float InValue)
{
	float NormalizedValue = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1.f), FVector2D(TargetSetting.Min, TargetSetting.Max), InValue);
	NormalizedValue = FMath::GetMappedRangeValueClamped(FVector2D(TargetSetting.Min, TargetSetting.Max), FVector2D(0.f, 1.f), NormalizedValue);
	SetTargetValue(NormalizedValue);
	ParentWidget->OnChangesDetected();
}

void UIndividualGamePlaySetting::SetTargetValue(float InValue)
{
	GameplaySettingSlider->SetValue(InValue);
	SetPercentageText(GameplaySettingSlider->GetValue());
}

void UIndividualGamePlaySetting::SetPercentageText(float InValue)
{
	float ValueLocal = GetPercentage(InValue, 0.f, 1.f);
	FText TextLocal = UKismetTextLibrary::Conv_FloatToText(ValueLocal, ERoundingMode::HalfToEven, false, true, 1, 3, 0, 0);
	PercentageText->SetText(TextLocal);
	SettingsValue = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1.f), FVector2D(TargetSetting.Min, TargetSetting.Max), InValue);
}

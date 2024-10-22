// Copywritten by Two Neurons Studio, LLC. All rights reserved.


#include "Menu/IndividualAudioSelector.h"
#include "Menu/AudioSettingMenu.h"
#include "Kismet/KismetTextLibrary.h"

bool UIndividualAudioSelector::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	AudioSettingSlider->OnValueChanged.Clear();
	AudioSettingSlider->OnValueChanged.AddUniqueDynamic(this, &UIndividualAudioSelector::UpdateSlider);

	return true;
}

void UIndividualAudioSelector::NativeConstruct()
{
	SetTargetAudioText();
	SetPercentageText(AudioSettingSlider->GetValue());
	SettingsValue = AudioSettingSlider->GetValue();
}

void UIndividualAudioSelector::SetTargetAudioText()
{
	switch (AudioTarget)
	{
	case AS_Master:
		TargetAudioText->SetText(FText::FromString("Master"));
		break;
	case AS_Ambient:
		TargetAudioText->SetText(FText::FromString("Ambient"));
		break;
	case AS_Dialogue:
		TargetAudioText->SetText(FText::FromString("Dialogue"));
		break;
	case AS_Effects:
		TargetAudioText->SetText(FText::FromString("Effects"));
		break;
	case AS_Music:
		TargetAudioText->SetText(FText::FromString("Music"));
		break;
	default:
		TargetAudioText->SetText(FText::FromString("Error!"));
		break;
	}
}

void UIndividualAudioSelector::SetParentWidget(UAudioSettingMenu* ParentWidgetIn)
{
	ParentWidget = ParentWidgetIn;
}

void UIndividualAudioSelector::UpdateSlider(float InValue)
{
	SetTargetValue(InValue);
	ParentWidget->OnChangeDetected();
}

void UIndividualAudioSelector::SetTargetValue(float InValue)
{
	AudioSettingSlider->SetValue(InValue);
	SetPercentageText(AudioSettingSlider->GetValue());
	SettingsValue = GetRatio(AudioSettingSlider->GetValue(), 0.f, 1.f);
}

void UIndividualAudioSelector::SetPercentageText(float InValue)
{
	float ValueLocal = GetPercentage(InValue, 0.f, 1.f);
	FText TextLocal = UKismetTextLibrary::Conv_FloatToText(ValueLocal, ERoundingMode::HalfToEven, false, true, 1, 3, 0, 0);
	PercentageText->SetText(TextLocal);
}

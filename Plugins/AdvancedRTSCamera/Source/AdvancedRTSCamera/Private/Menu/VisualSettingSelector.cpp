// Copywritten by Two Neurons Studio, LLC. All rights reserved.


#include "Menu/VisualSettingSelector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Menu/IndividualSetting.h"


void UVisualSettingSelector::SetItemParentWidget(UIndividualSetting* ParentWidget)
{
	if (!ensure(ParentWidget != nullptr)) return;

	IndividualSetting = ParentWidget;
	return;
}

void UVisualSettingSelector::NativeConstruct()
{
	Super::NativeConstruct();

	// bind mouse click events (recommended to make the click mode precise in the WBP child class)
	DownButton->OnClicked.AddDynamic(this, &UVisualSettingSelector::OnDownButtonClicked);
	UpButton->OnClicked.AddDynamic(this, &UVisualSettingSelector::OnUpButtonClicked);

}

void UVisualSettingSelector::OnDownButtonClicked()
{
	// decrease value by 1 (if possible)
	OnChange(-1); 
}

void UVisualSettingSelector::OnUpButtonClicked()
{
	// increase value by 1 (if possible)
	OnChange(1);
}

void UVisualSettingSelector::OnChange(int ChangeBy)
{
	// Store previous value (used for check at end of this method)
	PreviousValue = SettingValue;

	// Update the Settings value (and clamp)
	SettingValue = UKismetMathLibrary::Clamp((SettingValue + ChangeBy), 0, 3);

	// update text
	SetValueText();

	if (SettingValue != PreviousValue)
	{
		// update settings
	IndividualSetting->OnUpdate();
	}

	return;
}

void UVisualSettingSelector::SetValue(int SettingValueIn)
{
	PreviousValue = SettingValue;
	SettingValue = SettingValueIn;

	SetValueText();
}

void UVisualSettingSelector::SetValueText()
{
	switch (SettingValue)
	{
	case 0:
		ValueText = FText::FromString("Low");
		break;
	case 1:
		ValueText = FText::FromString("Medium");
		break;
	case 2:
		ValueText = FText::FromString("High");
		break;
	case 3:
		ValueText = FText::FromString("Epic");
		break;
	case 4:
		ValueText = FText::FromString("Custom");
		break;
	default:
		ValueText = FText::FromString("Epic");
		break;
	}

	SettingText->SetText(ValueText);
}

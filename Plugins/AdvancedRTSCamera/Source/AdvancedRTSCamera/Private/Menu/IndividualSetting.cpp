// Copywritten by Two Neurons Studio, LLC. All rights reserved.


#include "Menu/IndividualSetting.h"
#include "Menu/VisualSettingsMenu.h"

void UIndividualSetting::NativeConstruct()
{
	Super::NativeConstruct();

	// Set text element
	SettingItemText->SetText(UEnum::GetDisplayValueAsText(SettingTarget));

	// Set selector widget owner
	SettingItemOption->SetItemParentWidget(this);
}

void UIndividualSetting::OnUpdate()
{
	SettingsMenu->UpdateValues(false, Index);
}

void UIndividualSetting::SetParentWidget(UVisualSettingsMenu* ParentWidget)
{
	if (!ensure(ParentWidget != nullptr)) return;

	SettingsMenu = ParentWidget;
}

void UIndividualSetting::SetIndex(int NewIndex)
{
	Index = NewIndex;
}

void UIndividualSetting::SetValue(int SettingValue)
{
	SettingItemOption->SetValue(SettingValue);
}

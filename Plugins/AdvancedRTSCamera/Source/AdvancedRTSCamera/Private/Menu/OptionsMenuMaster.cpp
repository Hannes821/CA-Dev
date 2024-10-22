// Copywritten by Two Neurons Studio, LLC. All rights reserved.


#include "Menu/OptionsMenuMaster.h"

void UOptionsMenuMaster::NativeConstruct()
{

	// bind mouse click events (recommended to make the click mode precise in the WBP child class)
	CancelButton->OnClicked.Clear();
	CancelButton->OnClicked.AddDynamic(this, &UOptionsMenuMaster::OnCancel);
	AcceptButton->OnClicked.Clear();
	AcceptButton->OnClicked.AddDynamic(this, &UOptionsMenuMaster::OnAccept);
	DefaultButton->OnClicked.Clear();
	DefaultButton->OnClicked.AddDynamic(this, &UOptionsMenuMaster::OnDefault);
}

void UOptionsMenuMaster::OnCancel_Implementation()
{
	if (bChangeDetected)
	{
		// Display popup
		ConfirmCancelMenu->SetVisibility(ESlateVisibility::Visible);
		// disable the setting screen while the popup is on screen
		SettingsVerticalAnchor->SetIsEnabled(false);
	}
	else
	{
		RemoveFromParent();
	}
}

void UOptionsMenuMaster::OnAccept_Implementation()
{
	bChangeDetected = false;
	// consider striking this line
	RemoveFromParent();
}

void UOptionsMenuMaster::OnDefault_Implementation()
{
	// consider striking this line
	RemoveFromParent();
}

void UOptionsMenuMaster::ForceClose_Implementation()
{
	// force the widget closed (used when there are changes that the user is disregarding).
	RemoveFromParent();
}

void UOptionsMenuMaster::ResumeEditing_Implementation()
{
	// Hide pop-up widget
	ConfirmCancelMenu->SetVisibility(ESlateVisibility::Hidden);
	// enable the setting options
	SettingsVerticalAnchor->SetIsEnabled(true);
	bChangeDetected = true;
}

void UOptionsMenuMaster::ResetChanges_Implementation()
{
	return;
}

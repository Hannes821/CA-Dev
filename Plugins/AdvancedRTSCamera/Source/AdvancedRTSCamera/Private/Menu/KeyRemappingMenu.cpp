// Copywritten by Two Neurons Studio, LLC. All rights reserved.


#include "Menu/KeyRemappingMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "EnhancedInputLibrary.h"
#include "Interfaces/InputSaveInterface.h"
#include "Systems/SettingsSaveMaster.h"

void UKeyRemappingMenu::NativeConstruct()
{
	Super::NativeConstruct();

	ConfirmCancelMenu->SetSettingScreen(this);
	ConfirmCancelMenu->SetVisibility(ESlateVisibility::Hidden);
	ConfirmResetMenu->SetSettingScreen(this);
	ConfirmResetMenu->SetVisibility(ESlateVisibility::Hidden);

	bChangeDetected = false;
	SettingsVerticalAnchor->SetIsEnabled(true);

	GameInst = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInst != nullptr)) return;
}

void UKeyRemappingMenu::InitMenu(UPlayerMappableInputConfig* PMI)
{
	MappableBindings = PMI;
	SpawnCategoryPanels();
	SpawnSelectors();
	DeleteUnusedCategories();
}

void UKeyRemappingMenu::DeleteUnusedCategories()
{
	TArray<URemappingCategoryAnchor*> CategoriesToDeleteLocal;

	// Identify the category panels to delete
	for (URemappingCategoryAnchor* Category : CategoryAnchors)
	{
		if (!Category->TriggerHasChildren()) // if it has no children
		{
			CategoriesToDeleteLocal.AddUnique(Category);
		}
	}
	
	// Delete the panels
	for (URemappingCategoryAnchor* Category : CategoriesToDeleteLocal)
	{
		CategoryAnchors.Remove(Category);
		Category->RemoveFromParent();
	}
}

void UKeyRemappingMenu::RespawnSelectors()
{
	for (URemappingCategoryAnchor* Category : CategoryAnchors)
	{
		Category->TriggerClearChildren();
	}
	InputSelectors.Empty();
	SpawnSelectors();
	// if old categories that show up should be gone, run DeleteUnusedCategories here!
}

void UKeyRemappingMenu::OnCancel_Implementation()
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
		AcceptButton->SetVisibility(ESlateVisibility::Collapsed); // only difference from parent. @TODO If we use custom saves for other settings may be worthwhile adding this to the parent
		RemoveFromParent();
	}
}

void UKeyRemappingMenu::OnAccept_Implementation()
{
	for (UIndividualKeyMap* KeySelector : InputSelectors)
	{
		KeySelector->AcceptChanges();
	}
	AcceptButton->SetVisibility(ESlateVisibility::Collapsed);
	IInputSaveInterface::Execute_TriggerSaveBindings(GameInst);
	bChangeDetected = false;
//	RemoveFromParent();
}

void UKeyRemappingMenu::OnDefault_Implementation()
{
	if (bChangeDetected || IInputSaveInterface::Execute_GetSavedSettings(GameInst)->IsKeyIndexValid(0))
	{
		// changes detected, calls up change menu
		ConfirmResetMenu->SetVisibility(ESlateVisibility::Visible);
		SettingsVerticalAnchor->SetIsEnabled(false);
	}
	else
	{
		// no changes detected, resets just to avoid edge case issues with saves
		ResetChanges();
	}
}

void UKeyRemappingMenu::ForceClose_Implementation()
{
	if (bChangeDetected)
	{
		for (UIndividualKeyMap* KeySelector : InputSelectors)
		{
			KeySelector->CancelChanges();
		}
		bChangeDetected = false;
	}
	AcceptButton->SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromParent();
}

void UKeyRemappingMenu::ResetChanges_Implementation()
{
	ConfirmResetMenu->SetVisibility(ESlateVisibility::Hidden);

	for (UIndividualKeyMap* Selector : InputSelectors)
	{
		Selector->ResetKey();
	}
	IInputSaveInterface::Execute_TriggerSaveBindings(GameInst);
	SettingsVerticalAnchor->SetIsEnabled(true);
}


void UKeyRemappingMenu::OnUpdateValues()
{
	AcceptButton->SetVisibility(ESlateVisibility::Visible);
	bChangeDetected = true;
}

bool UKeyRemappingMenu::CheckValidity(FInputChord NewKey)
{
	bool bKeyInUseLocal{ false };
	for (UIndividualKeyMap* KeySelector : InputSelectors)
	{
		bKeyInUseLocal = NewKey == KeySelector->CurrentKey;
		if (bKeyInUseLocal)
		{
			break;
		}
	}
	return bKeyInUseLocal;
}

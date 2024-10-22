// Copywritten by Two Neurons Studio, LLC. All rights reserved.


#include "Menu/IndividualKeyMap.h"
#include "Kismet/KismetTextLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputLibrary.h"
#include "EnhancedInputSubsystemInterface.h"
#include "Framework/Commands/InputChord.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/InputSaveInterface.h"
#include "Interfaces/InputMapInterface.h"
#include "Menu/KeyRemappingMenu.h"

bool UIndividualKeyMap::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	KeySelector->OnIsSelectingKeyChanged.Clear();
	KeySelector->OnIsSelectingKeyChanged.AddUniqueDynamic(this, &UIndividualKeyMap::OnKeyChanged);
	KeySelector->OnKeySelected.Clear();
	KeySelector->OnKeySelected.AddUniqueDynamic(this, &UIndividualKeyMap::OnKeySelected);
	ResetButton->OnClicked.AddUniqueDynamic(this, &UIndividualKeyMap::OnResetKey);

	return true;
}

void UIndividualKeyMap::NativeConstruct()
{
	Super::NativeConstruct();

}

void UIndividualKeyMap::InitKeySelector(UKeyRemappingMenu* Parent, UPlayerMappableInputConfig* PMI, FEnhancedActionKeyMapping Map)
{
	if (!ensure(Parent != nullptr)) return;
	if (!ensure(PMI != nullptr)) return;

	SetSelectorVariables(Parent, PMI, Map);
	SetActionName();
	SetSelectorKeyValue();
	SetResetButton();
}

void UIndividualKeyMap::SetSelectorVariables(UKeyRemappingMenu* Parent, UPlayerMappableInputConfig* PMI, FEnhancedActionKeyMapping Map)
{
	ParentWidget = Parent;
	MappableBindings = PMI;
	KeyMapping = Map;
	CurrentKey = KeyMapping.Key;
	//TargetInput = KeyMapping.Action;
	GameInst = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInst != nullptr)) return;
	KeyBinds = IInputSaveInterface::Execute_GetSavedSettings(GameInst);

}

void UIndividualKeyMap::SetActionName()
{
	ActionName->SetText(KeyMapping.PlayerMappableOptions.DisplayName);
}

bool UIndividualKeyMap::SetSelectorKeyValue_Implementation()
{
	FKey KeyLocal;

	if (KeyBinds->IsMappingNameValid(KeyMapping.PlayerMappableOptions.Name))
	{
		// if an override exists use the override
		if(!bUseIcons)
		{
			KeySelector->SetSelectedKey(KeyBinds->GetKeyOverride(KeyBinds->FindMappingName(KeyMapping.PlayerMappableOptions.Name)));
		}
		return true;
	}
	else
	{
		// if an override does not exist, use the default
		if (!bUseIcons)
		{
			KeySelector->SetSelectedKey(CurrentKey);
		}
		return false;
	}
}

void UIndividualKeyMap::ReInit()
{
	SetActionName();
	SetSelectorKeyValue();
	SetResetButton();
}

void UIndividualKeyMap::SetResetButton()
{
	// enable or disenable revert button if a custom mapping exists
	if (KeyBinds->IsMappingNameValid(KeyMapping.PlayerMappableOptions.Name))
	{
		ResetButton->SetIsEnabled(true);
	}
	else
	{
		ResetButton->SetIsEnabled(false);
	}
}

void UIndividualKeyMap::OnKeyChanged()
{
	bRemapping = true;
	bPendingChanges = true;
	if(bUseIcons)
	{
		HideKeyIcon(true);
	}
}

void UIndividualKeyMap::OnKeySelected(FInputChord SelectedKey)
{
	NewKey = SelectedKey.Key;

	// make sure we are remapping and it is not the current key we selected
	if (bRemapping && NewKey != CurrentKey)
	{
		if (bUseIcons)
		{
			HideKeyIcon(false);
		}
		// Make sure the key is not alreayd in use
		if (ParentWidget->CheckValidity(SelectedKey))
		{
			// key is in use
			if (bUseIcons)
			{
				SetKeyIcon(CurrentKey);
			}
			else
			{
				KeySelector->SetSelectedKey(CurrentKey);
			}
		}
		else
		{
			if (bUseIcons)
			{
				SetKeyIcon(NewKey);
			}
			else
			{
				KeySelector->SetSelectedKey(NewKey);
			}
			ParentWidget->OnUpdateValues();
			ResetButton->SetIsEnabled(true);
		}
		bRemapping = false;
	}
}

void UIndividualKeyMap::OnResetKey()
{
	ResetKey();
}

void UIndividualKeyMap::AcceptChanges()
{
	bRemapping = false;
	bPendingChanges = false;

	APlayerController* PlayerControllerLocal = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//GetWorld()->GetFirstPlayerController();
	check(PlayerControllerLocal);

	TArray<TObjectPtr<UInputMappingContext>> KeysLocal;
	MappableBindings->GetMappingContexts().GetKeys(KeysLocal);
	for (TObjectPtr<UInputMappingContext> Key : KeysLocal)
	{
		for(size_t i = 0; i < Key->GetMappings().Num(); i++)
		{
			FEnhancedActionKeyMapping MappingsLocal = Key->GetMapping(i);
			// address edge case where user is remapping to same key (edge case as this should have been prevented earlier)
			if (MappingsLocal.Key == NewKey)
			{
				// do nothing
#if WITH_EDITOR
				UE_LOG(LogTemp, Warning, TEXT("Widget (Individual Key Map) - skipped binding; key already used"))
#endif
			}
			else
			{
				// set the new key when correct index in map is found and is not empty!
				int KeyIndex = Key->GetMappings().Find(KeyMapping);
				if (i == KeyIndex && NewKey.IsValid())
				{
					FEnhancedActionKeyMapping MapLocal = Key->GetMapping(KeyIndex);
					if (UEnhancedInputLocalPlayerSubsystem* EISubSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControllerLocal->GetLocalPlayer()))
					{
						FModifyContextOptions ContextLocal;
						ContextLocal.bIgnoreAllPressedKeysUntilRelease = true;
						ContextLocal.bForceImmediately = true;
						EISubSys->AddPlayerMappedKey(MapLocal.PlayerMappableOptions.Name, NewKey, ContextLocal);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Widget (Individual Key Map) - Failed to find Enhanced Input Subsystem on Accept Changes"));
					}

					// save the key (either by updating an existing custom keymap or adding one in)
					if (KeyBinds->IsMappingNameValid(KeyMapping.PlayerMappableOptions.Name))
					{
						int MapIndex = KeyBinds->FindMappingName(MapLocal.PlayerMappableOptions.Name);
						IInputSaveInterface::Execute_UpdateKeyBindings(GameInst, MapLocal.PlayerMappableOptions.Name, NewKey, MapIndex);
					}
					else
					{
						IInputSaveInterface::Execute_AddKeyBinding(GameInst, MapLocal.PlayerMappableOptions.Name, NewKey);
					}
				}
			}
		}
	}

}

void UIndividualKeyMap::CancelChanges()
{
	bRemapping = false;
	if (bPendingChanges)
	{
		// prevent deleting a custom key when a new custom key is temporarily set
		if(!KeyBinds->IsMappingNameValid(KeyMapping.PlayerMappableOptions.Name)) // no custom key already exists
		{
			ResetKey();
		}
		bPendingChanges = false;
	}
}

void UIndividualKeyMap::ResetKey()
{
	// Remove the save for the custom bind (if one exists)
	if (KeyBinds->IsMappingNameValid(KeyMapping.PlayerMappableOptions.Name))
	{
		IInputSaveInterface::Execute_RemoveBindingByIndex(GameInst, KeyBinds->FindMappingName(KeyMapping.PlayerMappableOptions.Name));
	}
	APlayerController* PlayerControllerLocal = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//GetWorld()->GetFirstPlayerController();
	check(PlayerControllerLocal);
	APawn* PlayerPawnLocal = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (UEnhancedInputLocalPlayerSubsystem* EISubSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControllerLocal->GetLocalPlayer()))
	{
		FModifyContextOptions ContextLocal;
		ContextLocal.bIgnoreAllPressedKeysUntilRelease = true;
		ContextLocal.bForceImmediately = true;
		EISubSys->RemovePlayerMappedKey(KeyMapping.PlayerMappableOptions.Name, ContextLocal);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Widget (Individual Key Map) - Failed to find Enhanced Input Subsystem on Reset"));
	}

	// update bindigns on controller OR pawn
	if (PlayerControllerLocal->GetClass()->ImplementsInterface(UInputMapInterface::StaticClass()))// Determine if Controller implements InputMapInterface
	{
		IInputMapInterface::Execute_ForceRebuildInputs(PlayerControllerLocal);
	}
	else if (PlayerPawnLocal->GetClass()->ImplementsInterface(UInputMapInterface::StaticClass()))// Determine if Player Pawn implements InputMapInterface
	{
		IInputMapInterface::Execute_ForceRebuildInputs(PlayerPawnLocal);
	}
	else // error state
	{
		UE_LOG(LogTemp, Error, TEXT("Widget (Individual Key Map) - Neither player nor actor implement interface"));
	}
	ReInit();
}

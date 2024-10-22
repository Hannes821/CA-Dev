// Copyright Two Neurons Studio, LLC. Year of Publication 2024.


#include "Menu/GameplaySettingsMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Interfaces/GameplaySettingsSaveInterface.h"
#include "Menu/CustomSaveSysLog.h"


bool UGameplaySettingsMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	return true;
}

void UGameplaySettingsMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameInst = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInst != nullptr)) return;

	bChangeDetected = false;
	ConfirmCancelMenu->SetVisibility(ESlateVisibility::Hidden);
	ConfirmCancelMenu->SetSettingScreen(this);
	
	CreateSelectorArray();

	bChangeDetected = false;
	SettingsVerticalAnchor->SetIsEnabled(true);
	AcceptButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UGameplaySettingsMenu::OnCancel_Implementation()
{
	if (bChangeDetected)
	{
		// Display pop-up
		ConfirmCancelMenu->SetVisibility(ESlateVisibility::Visible);
		// disable the settings screen while pop-up is visible/on screen
		SettingsVerticalAnchor->SetIsEnabled(true);
	}
	else
	{
		AcceptButton->SetVisibility(ESlateVisibility::Collapsed);
		RemoveFromParent();
	}
}

void UGameplaySettingsMenu::OnAccept_Implementation()
{
	if (bChangeDetected)
	{
		ApplyGameplaySettings();
	}
	AcceptButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UGameplaySettingsMenu::OnDefault_Implementation()
{
	size_t i{ 0 };
	for (UIndividualGamePlaySetting* Selector : GameplaySettingSelectors)
	{
		Selector->InitSlider(GameplayDefaults[i]);
		i++;
	}
	OnChangesDetected();
}

void UGameplaySettingsMenu::InitSettings_Implementation(const TArray<FMovementSettings>& SettingsIn, TArray<float>& GameplayDefaultsIn)
{
	StoredSettings = SettingsIn;
	GameplayDefaults = GameplayDefaultsIn;
}

void UGameplaySettingsMenu::PassSettings_Implementation(UIndividualGamePlaySetting* Selector, FMovementSettings SettingsIn)
{
	Selector->SetTargetSetting(SettingsIn);
}

void UGameplaySettingsMenu::LoadGameplaySettings_Implementation()
{
	size_t i{ 0 };
	for (UIndividualGamePlaySetting* Selector : GameplaySettingSelectors)
	{
		Selector->SetParentWidget(this);
		PassSettings(Selector, StoredSettings[i]);
		Selector->InitSlider(StoredSettings[i].Current);
		i++;
	}
}

void UGameplaySettingsMenu::OnChangesDetected()
{
	AcceptButton->SetVisibility(ESlateVisibility::Visible);
	bChangeDetected = true;
}

void UGameplaySettingsMenu::ApplyGameplaySettings_Implementation()
{
	size_t i{ 0 };
	for (UIndividualGamePlaySetting* Selector : GameplaySettingSelectors)
	{
		if (GameInst->GetClass()->ImplementsInterface(UGameplaySettingsSaveInterface::StaticClass()))
		{
			float ConvertedSettingLocal = Selector->SettingsValue;
			StoredSettings[i].Current = ConvertedSettingLocal;
			i++;
			if (!IGameplaySettingsSaveInterface::Execute_TriggerAdjustGameplaySettings(GameInst, Selector->SettingTarget, ConvertedSettingLocal))
			{
#if WITH_EDITOR
				UE_LOG(MenuSysLog, Error, TEXT("Gameplay Settings Menu - failed"));
#endif
			}
		}
		else
		{
#if WITH_EDITOR
			UE_LOG(MenuSysLog, Error, TEXT("Gameplay Settings Menu - game inst does not implment GameplaySettingsSaveInterface"));
#endif
		}
	}

	if (GameInst->GetClass()->ImplementsInterface(UGameplaySettingsSaveInterface::StaticClass()))
	{
		if (!IGameplaySettingsSaveInterface::Execute_ForceSaveGameplaySettings(GameInst))
		{
#if WITH_EDITOR
			UE_LOG(MenuSysLog, Error, TEXT("Gameplay Settings Menu - failed to save"));
#endif
		}
	}
}

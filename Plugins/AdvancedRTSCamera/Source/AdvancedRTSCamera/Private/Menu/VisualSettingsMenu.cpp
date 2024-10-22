// Copywritten by Two Neurons Studio, LLC. All rights reserved.


#include "Menu/VisualSettingsMenu.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Menu/IndividualSetting.h"

void UVisualSettingsMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Create object for settings
	Settings = UGameUserSettings::GetGameUserSettings();
	if (!ensure(Settings != nullptr)) return;

	// bind mouse click events (recommended to make the click mode precise in the WBP child class)
//	CancelButton->OnClicked.AddDynamic(this, &UVisualSettingsMenu::OnCancel);
//	AcceptButton->OnClicked.AddDynamic(this, &UVisualSettingsMenu::OnAccept);
//	AutoDetectButton->OnClicked.AddDynamic(this, &UVisualSettingsMenu::OnDetect);
	ScreenModeOptions->OnSelectionChanged.AddDynamic(this, &UVisualSettingsMenu::OnUpdateScreenMode);
	ResolutionOptions->OnSelectionChanged.AddDynamic(this, &UVisualSettingsMenu::OnUpdateResolution);
	VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &UVisualSettingsMenu::OnUpdateVSync);

	// confirm status of change detected is false.
	bChangeDetected = false;
	// Ensure the scroll panel is enabled
	SettingsVerticalAnchor->SetIsEnabled(true);
	// confirm cancel stuff here
	ConfirmCancelMenu->SetSettingScreen(this);
	ConfirmCancelMenu->SetVisibility(ESlateVisibility::Hidden);


	// Set up ScreenModeOptions options.
	if (bAllowFullscreen)
		ScreenModeOptions->AddOption("Fullscreen");
	if (bAllowWindowedFullScreen)
		ScreenModeOptions->AddOption("Bordless Fullscreen");
	if (bAllowWindowed)
		ScreenModeOptions->AddOption("Windowed");
	// Set default option
	ScreenModeOptions->SetSelectedOption("Fullscreen");

	// Set up data
	if (!VisualSettingOptions.IsValidIndex(0))
	{
		VisualSettingOptions.AddUnique(OverallQualitySelector);
		VisualSettingOptions.AddUnique(AntiAliasingSelector);
		VisualSettingOptions.AddUnique(FoliageSelector);
		VisualSettingOptions.AddUnique(GlobalIlluminationSelector);
		VisualSettingOptions.AddUnique(PostProcessingSelector);
		VisualSettingOptions.AddUnique(ReflectionsSelector);
		VisualSettingOptions.AddUnique(ShadingSelector);
		VisualSettingOptions.AddUnique(ShadowsSelector);
		VisualSettingOptions.AddUnique(TextureSelector);
		VisualSettingOptions.AddUnique(ViewDistanceSelector);
		VisualSettingOptions.AddUnique(VisualEffectsSelector);

		InitVideoSettings();
	}
}

void UVisualSettingsMenu::InitVideoSettings()
{
	// Get, load and validate/fix settings
	Settings->LoadSettings(true);
	Settings->ValidateSettings();

	// Store the selectors and set parent widgets
	FullyInitVideoSettings();

	// Set screen and resolution options (fill lists for latter and select current for both)
	SetScreenTypeOptions();
	SetResolutionOptions();

	// Select the resolution and set the scallability settings (currnet)
	GetDefaultVideoSettings();
}

void UVisualSettingsMenu::FullyInitVideoSettings()
{
	for (size_t i = 0; i < VisualSettingOptions.Num() - 1; i++)
	{
		//VideoSettings.Add(VisualSettingOptions[i]->GetSettingItemOption());
		VisualSettingOptions[i]->SetIndex(i);
		VisualSettingOptions[i]->SetParentWidget(this);
	}
}

void UVisualSettingsMenu::SetScreenTypeOptions()
{
	// declare (and set where possible) local variables.
	auto WindowModesLocal = Settings->GetFullscreenMode();
	FString TextLocal{ "Error" }; // if this seen then there is an error

	switch (WindowModesLocal)
	{
	case EWindowMode::Fullscreen:
		TextLocal = "Fullscreen";
		SelectedWindowMode = EScallabilityWindowMode::FullscreenWM;
		break;
	case EWindowMode::WindowedFullscreen:
		TextLocal = "Bordless Fullscreen";
		SelectedWindowMode = EScallabilityWindowMode::WindowedFullScreenFM;
		break;
	case EWindowMode::Windowed:
		TextLocal = "Windowed";
		SelectedWindowMode = EScallabilityWindowMode::WindowedFM;
		break;
	case EWindowMode::NumWindowModes:
		break;
	default:
		TextLocal = "Fullscreen";
		SelectedWindowMode = EScallabilityWindowMode::FullscreenWM;
		break;
	}

	// Set selected option
	ScreenModeOptions->SetSelectedOption(TextLocal);
}

void UVisualSettingsMenu::SetResolutionOptions()
{
	TArray<FIntPoint> ResolutionsLocal;
	ResolutionOptions->ClearOptions();
	
	ResolutionsLocal.Empty();
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(ResolutionsLocal);

	// NOTE: if this does not work as written copy the switch from above and fill the ResolutionsLocal in the switch

	for (FIntPoint Resolution : ResolutionsLocal)
	{
		FString ResolutionTextLocal = FString::FromInt(Resolution.X);
		ResolutionTextLocal.Append("x");
		ResolutionTextLocal.Append(FString::FromInt(Resolution.Y));
		ResolutionOptions->AddOption(ResolutionTextLocal);
	}
}

void UVisualSettingsMenu::GetDefaultVideoSettings()
{
	if (Settings->GetOverallScalabilityLevel() != -1)
	{
		// set overall and make sure all other values match if using overall
		VisualSettingOptions[0]->SetValue(Settings->GetOverallScalabilityLevel());
		Settings->SetOverallScalabilityLevel(Settings->GetOverallScalabilityLevel());
	}
	else
	{
		// if overall is custom, set individual values
		VisualSettingOptions[0]->SetValue(CUSTOM);
	}
	for (UIndividualSetting* SettingOption : VisualSettingOptions)
	{
		SettingOption->SettingTarget;
		switch (SettingOption->SettingTarget)
		{
		case AntiAliasing:
			SettingOption->SetValue(Settings->GetAntiAliasingQuality());
			break;
		case Foliage:
			SettingOption->SetValue(Settings->GetFoliageQuality());
			break;
		case GlobalIllumination:
			SettingOption->SetValue(Settings->GetGlobalIlluminationQuality());
			break;
		case PostProcessing:
			SettingOption->SetValue(Settings->GetPostProcessingQuality());
			break;
		case Reflections:
			SettingOption->SetValue(Settings->GetReflectionQuality());
			break;
		case Shading:
			SettingOption->SetValue(Settings->GetShadingQuality());
			break;
		case Shadows:
			SettingOption->SetValue(Settings->GetShadowQuality());
			break;
		case Textures:
			SettingOption->SetValue(Settings->GetTextureQuality());
			break;
		case ViewDistance:
			SettingOption->SetValue(Settings->GetViewDistanceQuality());
			break;
		case VisualEffects:
			SettingOption->SetValue(Settings->GetVisualEffectQuality());
			break;
		}
	}
		
		VSyncCheckBox->SetIsChecked(Settings->IsVSyncEnabled());

		FIntPoint CurrentResLocal = Settings->GetScreenResolution();
		FString ResolutionTextLocal = FString::FromInt(CurrentResLocal.X);
		ResolutionTextLocal.Append(RES);
		ResolutionTextLocal.Append(FString::FromInt(CurrentResLocal.Y));
		ResolutionOptions->SetSelectedOption(ResolutionTextLocal);

}

void UVisualSettingsMenu::UpdateValues(bool Bypass, int Index)
{
	// Make accept changes button visible and prevent the menu from being closed by accident
	AcceptButton->SetVisibility(ESlateVisibility::Visible);
	bChangeDetected = true;

	// exit out of method if resolution, vsync, or window mode is changed
	if (Bypass) return;

	// update the visual scallability values
	if (Index == 0) // if overall update all values
	{
		for (UIndividualSetting* SettingOption : VisualSettingOptions)
		{
			SettingOption->SetValue(VisualSettingOptions[0]->GetValue());
		}
	}
	else // if not overall set overall to custom
	{
		VisualSettingOptions[0]->SetValue(CUSTOM);
	}
}

void UVisualSettingsMenu::ApplyVideoSettings()
{
	// Apply all settings
	SetVSync();
	SetWindowedMode();
	SetScreenResolution();
	SetScallabilityOptions();
}

void UVisualSettingsMenu::SetVSync()
{
	Settings->SetVSyncEnabled(VSyncCheckBox->IsChecked());
}

void UVisualSettingsMenu::SetWindowedMode()
{
	switch (SelectedWindowMode)
	{
	case FullscreenWM:
		Settings->SetFullscreenMode(EWindowMode::Fullscreen);
		break;
	case WindowedFM:
		Settings->SetFullscreenMode(EWindowMode::Windowed);
		break;
	case WindowedFullScreenFM:
		Settings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		break;
	default:
		Settings->SetFullscreenMode(EWindowMode::Fullscreen);
		break;
	}
}

void UVisualSettingsMenu::SetScreenResolution()
{
	// get the string
	FString ResolutionTextLocal = ResolutionOptions->GetSelectedOption();
	
	// split the string into the x and y segements
	FString XS, YS;
	ResolutionTextLocal.Split(RES, &XS, &YS);

	// convert the string X and string Y to ints
	FIntPoint ResolutionLocal;
	ResolutionLocal.X = UKismetStringLibrary::Conv_StringToInt(XS);
	ResolutionLocal.Y = UKismetStringLibrary::Conv_StringToInt(YS);

	// update resolution
	Settings->SetScreenResolution(ResolutionLocal);
}

void UVisualSettingsMenu::SetScallabilityOptions()
{
	for (UIndividualSetting* Elem : VisualSettingOptions)
	{
		int ValueLocal = Elem->GetValue();

		switch (Elem->SettingTarget)
		{
		case Overall:
			if (ValueLocal == CUSTOM)
				Settings->SetOverallScalabilityLevel(CUSTOM);
			break;
		case AntiAliasing:
			Settings->SetAntiAliasingQuality(ValueLocal);
			break;
		case Foliage:
			Settings->SetFoliageQuality(ValueLocal);
			break;
		case GlobalIllumination:
			Settings->SetGlobalIlluminationQuality(ValueLocal);
			break;
		case PostProcessing:
			Settings->SetPostProcessingQuality(ValueLocal);
			break;
		case Reflections:
			Settings->SetReflectionQuality(ValueLocal);
			break;
		case Shading:
			Settings->SetShadingQuality(ValueLocal);
			break;
		case Shadows:
			Settings->SetShadowQuality(ValueLocal);
			break;
		case Textures:
			Settings->SetTextureQuality(ValueLocal);
			break;
		case ViewDistance:
			Settings->SetViewDistanceQuality(ValueLocal);
			break;
		case VisualEffects:
			Settings->SetVisualEffectQuality(ValueLocal);
			break;
		default:
			Settings->SetOverallScalabilityLevel(ValueLocal);
			break;
		}
	}

	Settings->ApplySettings(false);
}


void UVisualSettingsMenu::OnAccept_Implementation()
{
	ApplyVideoSettings();
	AcceptButton->SetVisibility(ESlateVisibility::Collapsed);
	bChangeDetected = false;
	// consider striking this line
	RemoveFromParent();
}

void UVisualSettingsMenu::OnDefault_Implementation()
{
	// Run benchmark with no updates
	Settings->RunHardwareBenchmark();
	VisualSettingOptions[0]->SetValue(Settings->GetOverallScalabilityLevel());
	UpdateValues();
}

void UVisualSettingsMenu::ForceClose_Implementation()
{
	// force the widget closed (used when there are changes that the user is disregarding).
	RemoveFromParent();
}

void UVisualSettingsMenu::ResumeEditing_Implementation()
{
	// Hide pop-up widget
	ConfirmCancelMenu->SetVisibility(ESlateVisibility::Hidden);
	// enable the setting options
	SettingsVerticalAnchor->SetIsEnabled(true);
	bChangeDetected = true;
}

void UVisualSettingsMenu::OnUpdateScreenMode(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	switch (SelectionType)
	{
	case ESelectInfo::Type::OnKeyPress:
		UpdateValues(true);
		break;
	case ESelectInfo::Type::OnNavigation:
		break;
	case ESelectInfo::Type::OnMouseClick:
		UpdateValues(true);
		break;
	default:
		break;
	}
}

void UVisualSettingsMenu::OnUpdateResolution(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	switch (SelectionType)
	{
	case ESelectInfo::Type::OnKeyPress:
		UpdateValues(true);
		break;
	case ESelectInfo::Type::OnNavigation:
		break;
	case ESelectInfo::Type::OnMouseClick:
		UpdateValues(true);
		break;
	default:
		break;
	}
}

void UVisualSettingsMenu::OnUpdateVSync(bool IsChecked)
{
	UpdateValues(true);
}

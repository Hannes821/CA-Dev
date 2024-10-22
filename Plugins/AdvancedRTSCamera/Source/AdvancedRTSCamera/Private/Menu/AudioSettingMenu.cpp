// Copywritten by Two Neurons Studio, LLC. All rights reserved.


#include "Menu/AudioSettingMenu.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/AudioSaveInterface.h"


bool UAudioSettingMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	return true;
}

void UAudioSettingMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameInst = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInst != nullptr)) return;

	bChangeDetected = false;
	ConfirmCancelMenu->SetVisibility(ESlateVisibility::Hidden);
	ConfirmCancelMenu->SetSettingScreen(this);
	SettingsVerticalAnchor->SetIsEnabled(true);

	CreateSelectorArray();
	SetMasterVolume();
	LoadAudioSettings();
}

void UAudioSettingMenu::CreateSelectorArray()
{
	AudioSelectors.Add(MasterSetting); // MUST BE FIRST
	AudioSelectors.Add(AmbientSetting);
	AudioSelectors.Add(DialogueSetting);
	AudioSelectors.Add(EffectsSetting);
	AudioSelectors.Add(MusicSetting);
}

void UAudioSettingMenu::LoadAudioSettings()
{
	for (UIndividualAudioSelector* Selector : AudioSelectors)
	{
		Selector->SetParentWidget(this);

		TMap<TEnumAsByte<EAudioSettingsOptions>, float> MapLocal = IAudioSaveInterface::Execute_TriggerGetAudioSettings(GameInst);

		for (auto& Target : MapLocal)
		{
			if (Target.Key == Selector->AudioTarget)
			{
				Selector->SetTargetValue(MapLocal.FindRef(Target.Key));
				break; // this should only happen once per inner loop
			}
		}
	}
}

void UAudioSettingMenu::OnChangeDetected()
{
	AcceptButton->SetVisibility(ESlateVisibility::Visible);
	bChangeDetected = true;
}

void UAudioSettingMenu::OnAccept_Implementation()
{
	if (bChangeDetected)
	{
		SetMasterVolume();
	}
	ApplyAudioSettings();
}

void UAudioSettingMenu::OnDefault_Implementation()
{
	for (UIndividualAudioSelector* Selector : AudioSelectors)
	{
		Selector->SetTargetValue(1.f);
	}
	OnChangeDetected();
}

void UAudioSettingMenu::SetMasterVolume()
{
	MasterVolumeSetting = AudioSelectors[0]->SettingsValue;
}

void UAudioSettingMenu::ApplyAudioSettings()
{
	float SettingRawLocal, NewVolumeLocal;
	for (UIndividualAudioSelector* Selector : AudioSelectors)
	{
		SettingRawLocal = Selector->SettingsValue;
		NewVolumeLocal = SettingRawLocal * MasterVolumeSetting;

		switch (Selector->AudioTarget)
		{
		case AS_Master:
			break;
		case AS_Ambient:
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterMix, AmbientSoundClass, NewVolumeLocal, 1.f, 0.f, true);
			break;
		case AS_Dialogue:
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterMix, DialogueSoundClass, NewVolumeLocal, 1.f, 0.f, true);
			break;
		case AS_Effects:
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterMix, EffectSoundClass, NewVolumeLocal, 1.f, 0.f, true);
			break;
		case AS_Music:
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterMix, MusicSoundClass, NewVolumeLocal, 1.f, 0.f, true);
			break;
		default:
			break;
		}

		UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterMix);
		IAudioSaveInterface::Execute_TriggerAdjustAudioSetting(GameInst, Selector->AudioTarget, SettingRawLocal);

	}
}

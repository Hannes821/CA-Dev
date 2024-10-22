// Copyright Two Neurons Studio, LLC. Year of Publication 2024.


#include "Systems/RTSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/RTSMovementInterface.h"


void URTSGameInstance::Init()
{
	Super::Init();
	LoadSaveData();
}

void URTSGameInstance::Shutdown()
{
	SaveSettings();
	Super::Shutdown();
}

void URTSGameInstance::SaveSettings()
{
	if (!SettingSave)
	{
		SettingSave = Cast<USettingsSaveMaster>(UGameplayStatics::LoadGameFromSlot(GameSettings, 0));
	}
	if (!ensure(SettingSave != nullptr))
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Game Instance - can't save settings, no save exists!"));
#endif
		CreateSettingsSave();
		return;
	}
	if (!ensure(SettingSave != nullptr)) return;
	UE_LOG(LogTemp, Warning, TEXT("Game Instance - Address for SettingSave %p"), SettingSave); // to get memory address
	if (UGameplayStatics::SaveGameToSlot(SettingSave, GameSettings, 0))
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Game Instance - Succesful Save on Settings"));
#endif
	}
	else
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Game Instance - FAILED SAVE ON EDITOR"));
#endif
	}
}

void URTSGameInstance::LoadKeybindings()
{
	if (UGameplayStatics::DoesSaveGameExist(GameSettings, 0))
	{
		SettingSave = Cast<USettingsSaveMaster>(UGameplayStatics::LoadGameFromSlot(GameSettings, 0));
		if (SettingSave)
		{
			APlayerController* PlayerControllerLocal = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			// GetWorld()->GetFirstPlayerController();
			check(PlayerControllerLocal);

			for (size_t i = 0; i < SettingSave->GetKeyOverrides().Num(); i++)
			{
				if (UEnhancedInputLocalPlayerSubsystem* EISubSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControllerLocal->GetLocalPlayer()))
				{
					FModifyContextOptions ContextLocal;
					ContextLocal.bIgnoreAllPressedKeysUntilRelease = true;
					ContextLocal.bForceImmediately = true;
					EISubSys->AddPlayerMappedKey(SettingSave->GetMappingName(i), SettingSave->GetKeyOverride(i), ContextLocal);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Game Instance - Failed to Load Key bindings! --One should exist!	--Creating new Bindings"));
			CreateSettingsSave();
		}
	}
	else
	{
		CreateSettingsSave();
	}
}

void URTSGameInstance::CreateSettingsSave()
{
	SettingSave = Cast<USettingsSaveMaster>(UGameplayStatics::CreateSaveGameObject(USettingsSaveMaster::StaticClass()));
	if (SettingSave)
	{
		SaveSettings();
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Game Instance - New Setting save generated"));
#endif
	}
	else
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Game Instance - No present Save Generated"));
#endif	
	}
}

void URTSGameInstance::LoadSaveData()
{
	if (UGameplayStatics::DoesSaveGameExist(GameSettings, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Instance - Loaded Settings"));
		SettingSave = Cast<USettingsSaveMaster>(UGameplayStatics::LoadGameFromSlot(GameSettings, 0));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Instance - Failed to load Settings! --One should exist!	--Creating new Settings"));
		CreateSettingsSave();
	}
}

void URTSGameInstance::LoadAudioSettings()
{
	//if (!ensure(SettingSave != nullptr)) return;
	if (SettingSave)
	{
		TMap<TEnumAsByte<EAudioSettingsOptions>, float> AudioSettingsLocal = SettingSave->GetAudioSettings();
		float MasterLocal = AudioSettingsLocal.FindRef(EAudioSettingsOptions::AS_Master);

		if (!ensure(MasterMix != nullptr)) return;

		for (auto& Setting : AudioSettingsLocal)
		{
			float RawVolumeLocal = Setting.Value;
			float NewVolumeLocal = RawVolumeLocal * MasterLocal;
			switch (Setting.Key)
			{
			case AS_Master:
				UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterMix);
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
				UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterMix);
				break;
			}

			UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterMix);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Instance - Failed to Audio Settings! --One should exist!	--Creating new Audio Settings"));
		LoadSaveData();
		LoadAudioSettings();
	}
}

void URTSGameInstance::LoadGameplaySettings()
{
	//if (!ensure(SettingSave != nullptr)) return;
	if (SettingSave)
	{
		APawn* PlayerPawnLocal = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		check(PlayerPawnLocal); // if this fails holy god something is wrong...
		if (PlayerPawnLocal->GetClass()->ImplementsInterface(URTSMovementInterface::StaticClass()))
		{
			TMap<TEnumAsByte<EGameplaySettings>, float> GameplaySettingsLocal = SettingSave->GetGameplaySettings();

			for (auto& Setting : GameplaySettingsLocal)
			{
				switch (Setting.Key)
				{
				case GS_MovementSpeed:
					IRTSMovementInterface::Execute_PushMovementSpeed(PlayerPawnLocal, Setting.Value);
					break;
				case GS_PanSensitivity:
					IRTSMovementInterface::Execute_PushPanSensitivity(PlayerPawnLocal, Setting.Value);
					break;
				default:
					break;
				}
			}
		}
	}
	else
	{
		LoadSaveData();
		LoadGameplaySettings();
	}
}


USettingsSaveMaster* URTSGameInstance::GetSavedSettings_Implementation()
{
	return SettingSave;
}

void URTSGameInstance::TriggerLoadBindings_Implementation()
{
	LoadKeybindings();
}

void URTSGameInstance::TriggerSaveBindings_Implementation()
{
	SaveSettings();
}

void URTSGameInstance::UpdateKeyBindings_Implementation(const FName& Name, const FKey& Key, int Index)
{
	SettingSave->SetMappingName(Name, Index);
	SettingSave->SetKeyOverride(Key, Index);
}

void URTSGameInstance::AddKeyBinding_Implementation(const FName& Name, const FKey& Key)
{
	SettingSave->AddMappingName(Name);
	SettingSave->AddKeyOverride(Key);
}

void URTSGameInstance::RemoveBindingByIndex_Implementation(int Index)
{
	SettingSave->RemoveMappingNameAt(Index);
	SettingSave->RemoveKeyOverrideAt(Index);
}

bool URTSGameInstance::TriggerAdjustAudioSettings_Implementation(EAudioSettingsOptions Key, float NewValueIn)
{
	SettingSave->AdjustAudioSetting(Key, NewValueIn);
	return true;
}

bool URTSGameInstance::ForceSaveAudioSettings_Implementation()
{
	SaveSettings();
	return true;
}

TMap<TEnumAsByte<EAudioSettingsOptions>, float> URTSGameInstance::TriggerGetAudioSettings_Implementation()
{
	if (!ensure(SettingSave != nullptr)) nullptr;
	return SettingSave->GetAudioSettings();
}

void URTSGameInstance::TriggerLoadAudioSettings_Implementation()
{
	LoadAudioSettings();
}

bool URTSGameInstance::TriggerAdjustGameplaySettings_Implementation(EGameplaySettings Key, float NewValueIn)
{
	SettingSave->AdjustGameplaySetting(Key, NewValueIn);
	return true;
}

bool URTSGameInstance::ForceSaveGameplaySettings_Implementation()
{
	SaveSettings();
	LoadGameplaySettings();
	return true;
}

void URTSGameInstance::TriggerLoadGameplaySettings_Implementation()
{
	LoadGameplaySettings();
}

TMap<TEnumAsByte<EGameplaySettings>, float> URTSGameInstance::TriggerGetGameplaySettings_Implementation()
{
	if (!ensure(SettingSave != nullptr)) nullptr;
	return SettingSave->GetGameplaySettings();
}
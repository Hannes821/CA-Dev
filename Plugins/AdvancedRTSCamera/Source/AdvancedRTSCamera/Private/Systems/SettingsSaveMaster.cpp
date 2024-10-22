// Copyright Two Neurons Studio, LLC. Year of Publication 2024.


#include "Systems/SettingsSaveMaster.h"

USettingsSaveMaster::USettingsSaveMaster(const FObjectInitializer& ObjectInitializer)
{
    AudioSettingsSave.Add(EAudioSettingsOptions::AS_Master,          1.f);
    AudioSettingsSave.Add(EAudioSettingsOptions::AS_Ambient,         1.f);
    AudioSettingsSave.Add(EAudioSettingsOptions::AS_Dialogue,        1.f);
    AudioSettingsSave.Add(EAudioSettingsOptions::AS_Effects,         1.f);
    AudioSettingsSave.Add(EAudioSettingsOptions::AS_Music,           1.f);

    // adjust vlaues for your defaults
    GameplaySettingsSave.Add(EGameplaySettings::GS_MovementSpeed,   15.f);
    GameplaySettingsSave.Add(EGameplaySettings::GS_PanSensitivity,  5.f);
}

void USettingsSaveMaster::SetKeyOverride(const FKey& Key, size_t Index)
{
    KeyBindingsSave.KeyOverrides[Index] = Key;
}

void USettingsSaveMaster::AddKeyOverride(const FKey& Key)
{
    KeyBindingsSave.KeyOverrides.Add(Key);
}

void USettingsSaveMaster::RemoveKeyOverrideAt(size_t Index)
{
    KeyBindingsSave.KeyOverrides.RemoveAt(Index);
}

void USettingsSaveMaster::RemoveAllkeyOverrides()
{
    KeyBindingsSave.KeyOverrides.Empty();
}

TArray<FKey> USettingsSaveMaster::GetKeyOverrides()
{
    return KeyBindingsSave.KeyOverrides;
}

int USettingsSaveMaster::FindKeyOverride(const FKey& Key)
{
    return KeyBindingsSave.KeyOverrides.Find(Key);
}

FKey USettingsSaveMaster::GetKeyOverride(int Index)
{
    return KeyBindingsSave.KeyOverrides[Index];
}

bool USettingsSaveMaster::IsKeyOverrideValid(const FKey& Key)
{
    return FindKeyOverride(Key) != -1 ? true : false;
}

bool USettingsSaveMaster::IsKeyIndexValid(int Index)
{
    return KeyBindingsSave.KeyOverrides.IsValidIndex(Index);
}

void USettingsSaveMaster::SetMappingName(const FName& Name, size_t Index)
{
    KeyBindingsSave.MappingNames[Index] = Name;
}

void USettingsSaveMaster::AddMappingName(const FName& Name)
{
    KeyBindingsSave.MappingNames.Add(Name);
}

void USettingsSaveMaster::RemoveMappingNameAt(size_t Index)
{
    KeyBindingsSave.MappingNames.RemoveAt(Index);
}

void USettingsSaveMaster::RemoveAllMappingNames()
{
    KeyBindingsSave.MappingNames.Empty();
}

TArray<FName> USettingsSaveMaster::GetMappingNames()
{
    return KeyBindingsSave.MappingNames;
}

int USettingsSaveMaster::FindMappingName(const FName& Name)
{
    return KeyBindingsSave.MappingNames.Find(Name);
}

FName USettingsSaveMaster::GetMappingName(int Index)
{
    return KeyBindingsSave.MappingNames[Index];
}

bool USettingsSaveMaster::IsMappingNameValid(const FName& Name)
{
    return FindMappingName(Name) != -1 ? true : false;
}

bool USettingsSaveMaster::IsMappingNameIndexValid(int Index)
{
    return KeyBindingsSave.MappingNames.IsValidIndex(Index);
}

void USettingsSaveMaster::AdjustAudioSetting(EAudioSettingsOptions Key, float NewValueIn)
{
    float NewValueLocal = NewValueIn;
    if (NewValueLocal > 1.f)
    {
        NewValueLocal = 1.f;
    }
    else if (NewValueLocal < 0.f)
    {
        NewValueLocal = 0.f;
    }

    if (!AudioSettingsSave.Contains(Key))
    {
        AudioSettingsSave.Add(Key, NewValueLocal);
    }
    else
    {
        AudioSettingsSave.Emplace(Key, NewValueLocal);
    }
}

void USettingsSaveMaster::AdjustGameplaySetting(EGameplaySettings Key, float NewValueIn)
{
    float NewValueLocal = NewValueIn;

    if (!GameplaySettingsSave.Contains(Key))
    {
        GameplaySettingsSave.Add(Key, NewValueLocal);
    }
    else
    {
        GameplaySettingsSave.Emplace(Key, NewValueLocal);
    }
}


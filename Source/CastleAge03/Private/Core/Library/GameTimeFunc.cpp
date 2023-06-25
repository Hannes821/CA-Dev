// Code written by Zachary Parker for Feldfeste 


#include "Core/Library/GameTimeFunc.h"

UGameTimeFunc::UGameTimeFunc()
{
}

UGameTimeFunc::~UGameTimeFunc()
{
}

int UGameTimeFunc::GetGameSpeedInt(float GameSpeed)
{
	// CHANGE THE DEFAULT HERE IF YOU CHANGE THE DEFAULT IN GAMESTATE
	float DefaultGameSpeedLocal = 60.0f;

	float SpeedSettingLocal = GameSpeed / DefaultGameSpeedLocal;
	if (SpeedSettingLocal == 0.0f)
	{
		return 0;
	}
	else if (FMath::IsNearlyEqual(SpeedSettingLocal, 1.75f, 0.1f))
	{
		return 1;
	}
	else if (FMath::IsNearlyEqual(SpeedSettingLocal, 1.25f, 0.1f))
	{
		return 2;
	}
	else if (FMath::IsNearlyEqual(SpeedSettingLocal, 1.0f, 0.1f))
	{
		return 3;
	}
	else if (FMath::IsNearlyEqual(SpeedSettingLocal, 0.5f, 0.1f))
	{
		return 4;
	}
	else if (FMath::IsNearlyEqual(SpeedSettingLocal, 0.25f, 0.1f))
	{
		return 5;
	}
	else // defualt
	{
		return 3;
	}
}

float UGameTimeFunc::SetSpeedMultiplier(int CurrentSpeedSetting)
{
	switch (CurrentSpeedSetting)
	{
	case 0:
	{
		return 0.0f;
		break;
	}
	case 1:
	{
		return 1.75f;
		break;
	}
	case 2:
	{
		return 1.25f;
		break;
	}
	case 3: //  also default
	{
		return 1.0f;
		break;
	}
	case 4:
	{
		return 0.5f;
		break;
	}
	case 5:
	{
		return 0.25f;
		break;
	}
	default:
	{
		return 1.0f;
		break;
	}
	}
}

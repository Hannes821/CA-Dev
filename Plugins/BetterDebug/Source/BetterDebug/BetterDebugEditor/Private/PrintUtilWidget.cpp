// Copyright 2024, KampaiRaptor, All Rights Reserved.

#include "PrintUtilWidget.h"
#include "PrintUtils.h"
#include "BetterDebug_HUD.h"


//Constructor
UPrintUtilWidget::UPrintUtilWidget()
{
}

//Destructor
UPrintUtilWidget::~UPrintUtilWidget()
{
}

//Get Bool array from PrintUtils Bp Library
TArray<bool>& UPrintUtilWidget::GetBoolArrayFromPrintUtils() const
{
	return UPrintUtils::BoolArray;
}

FDefaultPrintSetting UPrintUtilWidget::GetDefaultSetting()
{
	FDefaultPrintSetting Settings;
	Settings.GlobalViewport = UPrintUtils::bGlobalViewport;
	Settings.GlobalLOG = UPrintUtils::bGlobalLOG;
	return Settings;
}

//Set bool in array
bool UPrintUtilWidget::SetBoolInArray(bool Value, int32 Index)
{
	TArray<bool>& LocalBoolArray = UPrintUtils::BoolArray;
	
	if (LocalBoolArray.IsValidIndex(Index))
	{
		LocalBoolArray[Index] = Value;
		return true;
	}
	return false;
}

//Set Text Scale Multiplayer
void UPrintUtilWidget::SetTextScaleMultiplayer(float NewValue)
{
	UPrintUtils::TextScaleMultiplayer = NewValue;
}

void UPrintUtilWidget::SetGlobalBoolViewport(bool NewValue)
{
	UPrintUtils::bGlobalViewport = NewValue;
}

void UPrintUtilWidget::SetGlobalBoolLOG(bool NewValue)
{
	UPrintUtils::bGlobalLOG = NewValue;
}

void UPrintUtilWidget::PositionOnHUD(FVector2D NewPosition)
{
	UPrintUtils::PositionOnHUD = NewPosition;
}


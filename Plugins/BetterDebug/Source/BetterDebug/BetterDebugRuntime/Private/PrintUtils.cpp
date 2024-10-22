// Copyright 2024, KampaiRaptor, All Rights Reserved.

#include "PrintUtils.h"
#include "BetterDebug_HUD.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"


// Print String
void UPrintUtils::BetterPrintString(const FString& DebugInfo, const FString& Debug, const FString& Key, const int DebugIndex, const FPrintSetting Settings)
{
	//Display or not
	if (UPrintUtils::BoolArray[FMath::Clamp(DebugIndex, 0, 4)]) //Lock to 4 categories for now
	{

		FString const DisplayString = DebugInfo.IsEmpty() ? Debug : DebugInfo + ": " + Debug;
		
		//Check if HUD is setup, print to it if yes
		if (WorldContext)
		{
			FString const TempKey = !Key.Equals(TEXT("None"), ESearchCase::IgnoreCase) ? FString::FromInt(static_cast<int32>(GetTypeHash(Key))): FString::FromInt(FDateTime::Now().GetTicks());
			
			if (const APlayerController* PlayerController = WorldContext->GetFirstPlayerController())
			{
				ABetterDebug_HUD* HUD = Cast<ABetterDebug_HUD>(PlayerController->GetHUD());
				if (HUD != nullptr)
				{
					FHUDMessage HUDMessage;
					HUDMessage.Message = DisplayString;
					HUDMessage.Color = Settings.Color.ToFColor(true);
					HUDMessage.Position = PositionOnHUD;
					HUDMessage.TimeToDisplay = Settings.DisplayTime;
					HUDMessage.TextScale = Settings.TextSize * Settings.TextSize*TextScaleMultiplayer; 
					HUDMessage.TimeRemaining = Settings.DisplayTime;
			
					HUD->AddMessage(TempKey, HUDMessage);
				}
			}
		}
		//Or print on regular screen
		else
		{
			int32 const TempKey = !Key.Equals(TEXT("None"), ESearchCase::IgnoreCase) ? static_cast<int32>(GetTypeHash(Key)) : -1;

			if (Settings.bViewport && bGlobalViewport)
			{
				GEngine->AddOnScreenDebugMessage(TempKey, Settings.DisplayTime, Settings.Color.ToFColor(true), DisplayString, true, FVector2d(Settings.TextSize*TextScaleMultiplayer));
			}
		}
		
		//Print to LOG
		if (Settings.bLOG && bGlobalLOG)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *DisplayString);
		}
	}
}

// Print Bool
void UPrintUtils::PrintBool(const FString& DebugInfo, const bool& Value, const FString& Key, const int DebugIndex, const FPrintSetting Settings)
{
	BetterPrintString(DebugInfo, (Value ? TEXT("True") : TEXT("False")), Key, DebugIndex, Settings);
}

// Print Float
void UPrintUtils::PrintFloat(const FString& DebugInfo, const float& Debug, const FString& Key, const bool bRoundFloat, const int DebugIndex, const FPrintSetting Settings)
{
	const FString DisplayString = bRoundFloat ? FString::SanitizeFloat(FMath::RoundToFloat(Debug)) : FString::Printf(TEXT("%.2f"), Debug);
	BetterPrintString(DebugInfo, DisplayString, Key, DebugIndex, Settings);
}

// Print Int
void UPrintUtils::PrintInt(const FString& DebugInfo, const int& Debug, const FString& Key, const int DebugIndex, const FPrintSetting Settings)
{
	BetterPrintString(DebugInfo, FString::FromInt(Debug), Key, DebugIndex, Settings);
}

// Print String Array
void UPrintUtils::PrintStringArray(const FString& DebugInfo, const TArray<FString>& Array, const FString& Key, const int DebugIndex, const FPrintSetting Settings)
{
	FString DisplayString;
	for (int32 i = 0; i < Array.Num(); i++)
	{
		DisplayString += DebugInfo + " " + FString::FromInt(i) + ": " + Array[i] + "\n";
	}
	DisplayString.RemoveFromEnd("\n");
	BetterPrintString(DebugInfo, DisplayString, Key, DebugIndex, Settings);
}

// Print Transform
void UPrintUtils::PrintTransform(const FString& DebugInfo, const FTransform& Transform, const bool bLocation, const bool bRotation, const bool bScale, const FString& Key, const int DebugIndex, const FPrintSetting Settings)
{
	FString DisplayString = DebugInfo + ": \n";
	if (bLocation) DisplayString += "Location: " + Transform.GetLocation().ToString() + "\n";
	if (bRotation) DisplayString += "Rotation: " + Transform.GetRotation().Euler().ToString() + "\n";
	if (bScale) DisplayString += "Scale: " + Transform.GetScale3D().ToString();
	DisplayString.RemoveFromEnd("\n");
	BetterPrintString(DebugInfo, DisplayString, Key, DebugIndex, Settings);
}

// Print Vector
void UPrintUtils::PrintVector(const FString& DebugInfo, const FVector& Vector, const bool bX, const bool bY, const bool bZ, const FString& Key, const int DebugIndex, const FPrintSetting Settings)
{
	FString DisplayString = DebugInfo + ":";
	if (bX) DisplayString += " X: " + FString::SanitizeFloat(FMath::RoundToFloat(Vector.X));
	if (bY) DisplayString += " Y: " + FString::SanitizeFloat(FMath::RoundToFloat(Vector.Y));
	if (bZ) DisplayString += " Z: " + FString::SanitizeFloat(FMath::RoundToFloat(Vector.Z));
	DisplayString.RemoveFromEnd(" ");
	BetterPrintString(DebugInfo, DisplayString, Key, DebugIndex, Settings);
}

// Print Rotator
void UPrintUtils::PrintRotator(const FString& DebugInfo, const FRotator& Rotator, const bool bX_Roll, const bool bY_Pitch, const bool bZ_Yaw, const FString& Key, const int DebugIndex, const FPrintSetting Settings)
{
	FString DisplayString = DebugInfo + ":";
	if (bX_Roll) DisplayString += " X (Roll): " + FString::SanitizeFloat(Rotator.Roll);
	if (bY_Pitch) DisplayString += " Y (Pitch): " + FString::SanitizeFloat(Rotator.Pitch);
	if (bZ_Yaw) DisplayString += " Z (Yaw): " + FString::SanitizeFloat(Rotator.Yaw);
	DisplayString.RemoveFromEnd(" ");
	BetterPrintString(DebugInfo, DisplayString, Key, DebugIndex, Settings);
}

void UPrintUtils::PrintHere()
{
	FPrintSetting Settings;
	Settings.Color = FColor::Blue;
	BetterPrintString("", "Here", "None", 0, Settings);
}

void UPrintUtils::PrintWorks()
{
	FPrintSetting Settings;
	Settings.Color = FColor::Green;
	BetterPrintString("", "Works", "None", 0, Settings);
}

void UPrintUtils::PrintFail()
{
	FPrintSetting Settings;
	Settings.Color = FColor::Red;
	BetterPrintString("", "Fail", "None", 0, Settings);
}
// Copyright 2024, KampaiRaptor, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PrintUtils.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPrintSetting
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PrintSetting")
	FLinearColor Color = FLinearColor::Red;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PrintSetting")
	float DisplayTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PrintSetting")
	bool bViewport = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PrintSetting")
	bool bLOG = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PrintSetting")
	float TextSize = 1.2f;
	
};

UCLASS()
class BETTERDEBUGRUNTIME_API UPrintUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Debug") 
	static void BetterPrintString(const FString& DebugInfo, const FString& Debug, const FString& Key = "None", const int DebugIndex = 0,  const FPrintSetting Settings = FPrintSetting());

	UFUNCTION(BlueprintCallable, Category="Debug") 
	static void PrintBool(const FString& DebugInfo, const bool& Value, const FString& Key = "None", const int DebugIndex = 0,  const FPrintSetting Settings = FPrintSetting());
	
	UFUNCTION(BlueprintCallable, Category="Debug")
	static void PrintFloat(const FString& DebugInfo, const float& Debug, const FString& Key = "None", const bool bRoundFloat = false, const int DebugIndex = 0, const FPrintSetting Settings = FPrintSetting());

	UFUNCTION(BlueprintCallable, Category="Debug")
	static void PrintInt(const FString& DebugInfo, const int& Debug, const FString& Key = "None", const int DebugIndex = 0, const FPrintSetting Settings = FPrintSetting());
	
	UFUNCTION(BlueprintCallable, Category="Debug")
	static void PrintStringArray(const FString& DebugInfo, const TArray<FString>& Array, const FString& Key = "None", const int DebugIndex = 0, const FPrintSetting Settings = FPrintSetting());
	
	UFUNCTION(BlueprintCallable, Category="Debug")
	static void PrintTransform(const FString& DebugInfo, const FTransform& Transform, const bool bLocation = true, const bool bRotation = true, const bool bScale = true, const FString& Key = "None", const int DebugIndex = 0, const FPrintSetting Settings = FPrintSetting());
	
	UFUNCTION(BlueprintCallable, Category="Debug")
	static void PrintVector(const FString& DebugInfo, const FVector& Vector, const bool bX = true, const bool bY = true, const bool bZ = true, const FString& Key = "None", const int DebugIndex = 0, const FPrintSetting Settings = FPrintSetting());

	UFUNCTION(BlueprintCallable, Category="Debug")
	static void PrintRotator(const FString& DebugInfo, const FRotator& Rotator, const bool bX_Roll = true, const bool bY_Pitch = true, const bool bZ_Yaw = true, const FString& Key = "None", const int DebugIndex = 0, const FPrintSetting Settings = FPrintSetting());

	UFUNCTION(BlueprintCallable, Category="Debug")
	static void PrintHere();

	UFUNCTION(BlueprintCallable, Category="Debug")
	static void PrintWorks();

	UFUNCTION(BlueprintCallable, Category="Debug")
	static void PrintFail();
	
	static inline TArray<bool> BoolArray{true, true, true, true, true};

	static inline UWorld* WorldContext = nullptr;
	
	static inline float TextScaleMultiplayer = 1.0;
	static inline FVector2D PositionOnHUD = FVector2D(0.0f, 0.0f);

	static inline bool bGlobalViewport = true;
	static inline bool bGlobalLOG = true;	
};

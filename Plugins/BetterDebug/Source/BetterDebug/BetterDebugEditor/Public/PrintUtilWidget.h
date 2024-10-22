// Copyright 2024, KampaiRaptor, All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "PrintUtilWidget.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDefaultPrintSetting
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "PrintSetting")
	bool GlobalViewport;

	UPROPERTY(BlueprintReadOnly, Category = "PrintSetting")
	bool GlobalLOG;
};

class UPrintUtils;

UCLASS()
class BETTERDEBUGEDITOR_API UPrintUtilWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
public:
	UPrintUtilWidget();
	~UPrintUtilWidget();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WidgetDebugUtils")
	TArray<bool>& GetBoolArrayFromPrintUtils() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WidgetDebugUtils")
	FDefaultPrintSetting GetDefaultSetting ();


	UFUNCTION(BlueprintCallable, Category = "WidgetDebugUtils")
	bool SetBoolInArray(bool Value, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "WidgetDebugUtils")
	void SetTextScaleMultiplayer(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "WidgetDebugUtils")
	void SetGlobalBoolViewport(bool NewValue);

	UFUNCTION(BlueprintCallable, Category = "WidgetDebugUtils")
	void SetGlobalBoolLOG(bool NewValue);

	UFUNCTION(BlueprintCallable, Category = "WidgetDebugUtils")
	void PositionOnHUD(FVector2D NewPosition);
	
};


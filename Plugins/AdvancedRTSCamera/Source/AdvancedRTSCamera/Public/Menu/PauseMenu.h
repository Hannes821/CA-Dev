// Copyright Two Neurons Studio, LLC. Year of Publication 2024.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "KeyRemappingMenu.h"
#include "VisualSettingsMenu.h"
#include "AudioSettingMenu.h"
#include "Menu/GameplaySettingsMenu.h"
#include "Camera/MovementSettingsStructs.h"
#include "PauseMenu.generated.h"

/**
 * This is the pause menu - it houses all the options menus (and could house save menus).
 * It uses an interface to talk to the player to resume the game on button press.
 */
UCLASS()
class ADVANCEDRTSCAMERA_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

	friend class UPauseMenu;

/************************************************
* ATTRIBUTES
************************************************/
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/	
protected:
	/* Button to resume game */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* ResumeButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* GraphicsButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* BindingsButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* GameplayButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* AudioSettingsButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* QuitGameButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UWidget* DefaultBlank;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UKeyRemappingMenu* KeyRemapping;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UVisualSettingsMenu* VisualSettings;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UAudioSettingMenu* AudioSettings;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UGameplaySettingsMenu* GameplaySettings;

/************************************************
* METHODS
************************************************/
	/************************************************
	* INHERITED METHODS
	************************************************/	
	virtual void NativeConstruct() override;

	/************************************************
	* MEMBER METHODS
	************************************************/
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void InitSettings(UPlayerMappableInputConfig* PMI, TArray<FMovementSettings> GameplaySettingsIn, TArray<float> GameplayDefaultsIn); 
	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnClickResume();
	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnClickGraphics();
	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnClickBindings();
	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnClickAudio();
	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnClickGameplaySettings();

	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnClickQuit();
};

// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/SizeBox.h"
#include "TimerManager.h"
#include "ToolsMenu.generated.h"

/**
 * This class will house the bound functions for all tool based menus for the game
 * This class will have its main code written in C++ but will be bound to a WBP
 * This widget will be added to a parent widgets.
 * 
 * All widgets will be implemented by a BP derived player controller.
 */

UCLASS(Abstract)
class UToolsMenu : public UUserWidget
{
	GENERATED_BODY()

/************************************************
* CONSTRUCTORS
************************************************/
public:
	UToolsMenu (const FObjectInitializer& ObjectInitializer);

/************************************************
* ATTRIBUTES
************************************************/
protected:
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/
	//UPROPERTY(meta = (BindWidget))
	//class UButton* AimsButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConstructionButton;

	//UPROPERTY(meta = (BindWidget))
	//class UButton* OverviewButton;

	//UPROPERTY(meta = (BindWidget))
	//class UButton* ChatButton;

	//UPROPERTY(meta = (BindWidget))
	//class UButton* DiplomancyButton;

	//UPROPERTY(meta = (BindWidget))
	//class UButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HouseButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* GrannaryButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* ConstructionSwitcher;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* ConstructionsMenuSizeBox;

	/************************************************
	* VARIABLE ATTRIBUTES
	************************************************/
private:
	bool bIsConstructionMenuOpen{ false };


/************************************************
* METHODS
************************************************/
protected:
	bool Initialize();

	// Optionally override the Blueprint "Event Construct" event
	virtual void NativeConstruct() override;

	// Optionally override the tick event
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/************************************************
	* DISPLAY MENUS
	************************************************/
	//UFUNCTION(BlueprintCallable)
	//void DisplayAimsMenu();

	UFUNCTION(BlueprintCallable)
	void DisplayConstructionMenu();

	void ShowConstructionMenu(ESlateVisibility InVisibility);

	//UFUNCTION(BlueprintCallable)
	//void DisplayOverviewMenu();

	//UFUNCTION(BlueprintCallable)
	//void DisplayChat();

	//UFUNCTION(BlueprintCallable)
	//void DisplayDiplomacyMenu();

	//UFUNCTION(BlueprintCallable)
	//void DisplayOptionsMenu();


	/************************************************
	* CONSTRUCTION OPTIONS
	************************************************/
	void ConstructPreview(FName InRowName);

	UFUNCTION(BlueprintCallable)
	void BuildHouse();

	UFUNCTION(BlueprintCallable)
	void BuildGranary();
};

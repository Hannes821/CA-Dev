// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Misc/DateTime.h"
#include "Core/Settings/RTSGameState.h"
#include "Core/Interfaces/GameTimeInterface.h"
#include "GameClockWidget.generated.h"

/**
 * This is going to be the parent class for all game clock widgets.
 * This class will have its main code written in C++ but
 * in a derived WBP will have the visuals implemented.
 * This widget will be added to a parent widget
 * 
 * All widgets will be implemented by a BP derived player controller.
 * 
 */

UCLASS(Abstract)
class UGameClockWidget : public UUserWidget, public IGameTimeInterface
{
	GENERATED_BODY()

/************************************************
* CONSTRUCTORS
************************************************/
public:
	UGameClockWidget(const FObjectInitializer& ObjectInitializer);

/************************************************
* ATTRIBUTES
************************************************/
protected:
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/
	UPROPERTY(meta = (BindWidget))
	class UButton* IncreaseSpeedButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* DecreaseSpeedButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PlayPauseButton;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* GameTimeProgBar;

	/************************************************
	* VARIABLE ATTRIBUTES
	************************************************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Date And time")
	FDateTime DateTimeStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Date And time")
	int Day;

	/* for if you want seasons
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar Information")
		//ESeasons Seasons;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bIsPaused{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bools")
	bool bUse12HourClock { false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSpeed")
	int SpeedSetting{ 3 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSpeed")
	int StoredSpeedSetting{ 3 }; // this needs to match the default Speed setting above!

	UPROPERTY(Category = "References", VisibleAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	class APlayerController* PlayerControllerRef;

	UPROPERTY(Category = "References", VisibleAnywhere, BlueprintReadWrite)
	class ARTSGameState* GameStateRef;

/************************************************
* METHODS
************************************************/
public:
	void CheckButtons();

protected:
	bool Initialize();

	// Optionally override the Blueprint "Event Construct" event
	virtual void NativeConstruct() override;

	// Optionally override the tick event
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/* Set owning player*/
	UFUNCTION(BlueprintCallable, Category = "References")
	void SetPlayerControllerRef(APlayerController* Reference);

	UFUNCTION(BlueprintCallable)
	void RunIncreaseSpeed();

	UFUNCTION(BlueprintCallable)
	void RunDecreaseSpeed();

	UFUNCTION(BlueprintCallable)
	void RunPlayPause();

	/* Sets numeric value for day in a calendar */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Calendar", meta = (BindWidget))
	FText SetDayText();

	/* Sets a written out day (e.g., "Monday") */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Calendar", meta = (BindWidget))
	FText SetDayofWeekText();

	/* Sets written out version of month - using written out to avoid EU/US confusion */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Calendar", meta = (BindWidget))
	FText SetMonthText();

	/* Sets the numerix text for year*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Calendar", meta = (BindWidget))
	FText SetYearText();

	/* Sets the numeric text for seconds on a clock */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Clock", meta = (BindWidget))
	FText SetSecondText();

	/* Sets numeric text for minutes on a clock */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Clock", meta = (BindWidget))
	FText SetMinuteText();

	/* Sets Numeric value for hours on a clock - both 12 and 24 hour clock versions are available*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Clock", meta = (BindWidget))
	FText SetHourText();

	/* Sets AM or PM text - do not use if using a 24 hour clock */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Clock", meta = (BindWidget))
	FText SetTextAMPM();

	/* Creates a symbol for reducing speed */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Speed", meta = (BindWidget))
	FText SetSpeedReduceButtonText();

	/* Creates a symbol for play / pause */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Speed", meta = (BindWidget))
	FText SetPlayPauseButton();

	/* Creates a symbol for increasing speed */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Speed", meta = (BindWidget))
	FText SetSpeedIncraseButtonText();

	/* Event that passes the speed settings to player - uses interface to communicate to game state */
	UFUNCTION(BlueprintCallable, Category = "Speed")
	virtual void SetGameSpeed(int CurrentSpeedSetting);

	/* Player control over game speed; decrease - implemented via button press */
	UFUNCTION(BlueprintCallable, Category = "Speed")
	virtual void DecreaseSpeed();

	/* Player control over game speed; increase - implemented via button press */
	UFUNCTION(BlueprintCallable, Category = "Speed")
	virtual void IncreaseSpeed();

	/* Method for pausing and unpausing the game */
	UFUNCTION(BlueprintCallable, Category = "Speed")
	virtual void PlayPauseGame();


	/* Method for updating progress bar*/
	UFUNCTION(BlueprintCallable, Category = "Speed|ProgBar")
	virtual void SetGameTimeProgBar();

	/************************************************
	* INTERFACE RELATED
	************************************************/
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetGameDateTime(FDateTime TimeStruct); // prototype
	virtual bool SetGameDateTime_Implementation(FDateTime TimeStruct);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetGameDay(int GameDay); // prototype
	virtual bool SetGameDay_Implementation(int GameDay);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetGameSpeedInt(float GameSpeed);
	bool SetGameSpeedInt_Implementation(float GameSpeed);
};

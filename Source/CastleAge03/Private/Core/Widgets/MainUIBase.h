// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Widgets/GameClockWidget.h"
#include "Core/Widgets/ToolsMenu.h"
#include "Core/Interfaces/GameTimeInterface.h"
#include "Components/CanvasPanel.h"
#include "MainUIBase.generated.h"

/**
 * This is the main UI and stores all the data for the main UI.
 */
UCLASS()
class UMainUIBase : public UUserWidget, public IGameTimeInterface
{
	GENERATED_BODY()
	
/************************************************
* CONSTRUCTORS
************************************************/
public:
	//UMainUIBase(const FObjectInitializer& ObjectInitializer);

/************************************************
* ATTRIBUTES
************************************************/
public:
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) // original: Transient, EditAnywhere, BlueprintReadWrite ADDED ON:, Category = "Master|Child", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	UGameClockWidget* GameClock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UToolsMenu* ToolsMenu;

	/************************************************
	* VARIABLE ATTRIBUTES
	************************************************/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Date And time")
	FDateTime DateTimeStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Date And time")
	int Day;

/************************************************
* METHODS
************************************************/
protected:
	// Optionally override the Blueprint "Event Construct" event
	virtual void NativeConstruct() override;

	// Optionally override the tick event
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	virtual UGameClockWidget* GetClockWidget()
	{
		return GameClock;
	}

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

	UFUNCTION(BlueprintCallable, Category = "GameTimeWidget")
	void PassDateTimeStruct(FDateTime TimeStruct, UGameClockWidget* ClockIn);

	UFUNCTION(BlueprintCallable, Category = "GameTimeWidget")
	void PassDayOfWeek(int GameDay, UGameClockWidget* ClockIn);

	UFUNCTION(BlueprintCallable, Category = "GameTimeWidget")
	void PassGameSpeed(float GameSpeed, UGameClockWidget* ClockIn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetGameSpeedInt(float GameSpeed);
	bool SetGameSpeedInt_Implementation(float GameSpeed);
};

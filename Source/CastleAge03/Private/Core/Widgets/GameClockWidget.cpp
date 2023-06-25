// Code written by Zachary Parker for Feldfeste 


#include "Core/Widgets/GameClockWidget.h"
#include "Internationalization/Text.h"
#include "Core/Library/GameTimeFunc.h"
#include "UObject/Class.h"
#include "Engine/Engine.h"
#include "Engine/World.h"


UGameClockWidget::UGameClockWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Everything is initialized in header that we need to worry about.
	// The undeclared variables will be filled only at runtime (methods below address this)
}

bool UGameClockWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(IncreaseSpeedButton != nullptr)) return false;
	IncreaseSpeedButton->OnClicked.AddDynamic(this, &UGameClockWidget::RunIncreaseSpeed);

	if (!ensure(IncreaseSpeedButton != nullptr)) return false;
	DecreaseSpeedButton->OnClicked.AddDynamic(this, &UGameClockWidget::RunDecreaseSpeed);

	if (!ensure(IncreaseSpeedButton != nullptr)) return false;
	PlayPauseButton->OnClicked.AddDynamic(this, &UGameClockWidget::RunPlayPause);

	return true;
}

void UGameClockWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// populate GameStateRef
	GameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
	// populates owner
	APlayerController* ReferenceLocal = GetWorld()->GetFirstPlayerController();
	SetPlayerControllerRef(ReferenceLocal);
}

void UGameClockWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGameClockWidget::SetPlayerControllerRef(APlayerController* Reference)
{
	
	PlayerControllerRef = GetWorld()->GetFirstPlayerController();
}

/*
*  Below are the methods used for button events
* these are the onclicked events.
*/

void UGameClockWidget::CheckButtons()
{
	if (SpeedSetting == 0)
	{
		DecreaseSpeedButton->SetIsEnabled(false);
	}
	else
	{
		DecreaseSpeedButton->SetIsEnabled(true);
		if (SpeedSetting == 5)
		{
			IncreaseSpeedButton->SetIsEnabled(false);
		}
		else
		{
			IncreaseSpeedButton->SetIsEnabled(true);
		}
	}
}


void UGameClockWidget::RunIncreaseSpeed()
{
	IncreaseSpeed();
	CheckButtons();
}

void UGameClockWidget::RunDecreaseSpeed()
{
	DecreaseSpeed();
	CheckButtons();
}

void UGameClockWidget::RunPlayPause()
{
	PlayPauseGame();
	CheckButtons();
}

/*
* Below are the methods used to implementing the UMG widgets.
* These methods are mostly the binds for FText.
* FText lacks some of the required methods, however.
* So any FText method will have a string declared if needed 
* The string is used to convert ints or other values over.
*/

/* Sets numeric value for day in a calendar */
FText UGameClockWidget::SetDayText()
{
	//declare FString
	FString DayStringLocal;

	//Get int for day local and set string
	int DayLocal = DateTimeStruct.GetDay();
	DayStringLocal = FString::FromInt(DayLocal);

	//Set and return FString
	FText DayTextLocal = FText::FromString(DayStringLocal);
	return DayTextLocal;
}

/* Sets a written out day */
FText UGameClockWidget::SetDayofWeekText()
{
	//declare FString
	FString DayNameStringLocal;

	// Get name of day Local and set string
	/*
	* 1 = Sunday
	* 2 = Monday
	* 7 = Saturday
	* 
	*/
	switch(Day) // Day is declared in the header
	{
	case 1:
		{
			DayNameStringLocal = "Sun";
			break;
		}
	case 2:
		{
			DayNameStringLocal = "Mon";
			break;
		}
	case 3:
		{
			DayNameStringLocal = "Tue";
			break;
		}
	case 4:
		{
			DayNameStringLocal = "Wed";
			break;
		}
	case 5:
		{
			DayNameStringLocal = "Thu";
			break;
		}
	case 6:
		{
			DayNameStringLocal = "Fri";
			break;
		}
	case 7:
		{
			DayNameStringLocal = "Sat";
			break;
		}
	case 8:
		{
			DayNameStringLocal = "Welcome to the Tardis";
			break;
		}
	default:
		{
			DayNameStringLocal = "Please stop breaking time";
			break;
		}
	}

	// Set and return FString
	FText DayNameTextLocal = FText::FromString(DayNameStringLocal);
	return DayNameTextLocal;
}

/* Sets written out version of month - using written out to avoid EU/US confusion */
FText UGameClockWidget::SetMonthText()
{
	//declare FString
	FString MonthStringLocal;

	// If you need me to define how a numeric month converts to a name.... then we have issues
	// This does not account for localization so you might need to tell me the names!

	int MonthLocal = DateTimeStruct.GetMonth();
	switch (MonthLocal) // Day is declared in the header
	{
	case 1:
	{
		MonthStringLocal = "January";
		break;
	}
	case 2:
	{
		MonthStringLocal = "February";
		break;
	}
	case 3:
	{
		MonthStringLocal = "March";
		break;
	}
	case 4:
	{
		MonthStringLocal = "April";
		break;
	}
	case 5:
	{
		MonthStringLocal = "May";
		break;
	}
	case 6:
	{
		MonthStringLocal = "June";
		break;
	}
	case 7:
	{
		MonthStringLocal = "July";
		break;
	}
	case 8:
	{
		MonthStringLocal = "August";
		break;
	}
	case 9:
	{
		MonthStringLocal = "September";
		break;
	}
	case 10:
	{
		MonthStringLocal = "October";
		break;
	}
	case 11:
	{
		MonthStringLocal = "November";
		break;
	}
	case 12:
	{
		MonthStringLocal = "December";
		break;
	}
	case 13:
	{
		MonthStringLocal = "Smarch; do not touch Willie";
		break;
	}
	default:
	{
		MonthStringLocal = "Time is broken! It is Armageddon";
		break;
	}
	}

	// Set and return FString
	FText MonthTextLocal = FText::FromString(MonthStringLocal);
	return MonthTextLocal;
}

/* Sets the numerix text for year*/
FText UGameClockWidget::SetYearText()
{
	//declare FString
	FString YearStringLocal;

	//Get int for year local and set string
	int YearLocal = DateTimeStruct.GetYear();
	YearStringLocal = FString::FromInt(YearLocal);

	//Set and return FString
	FText YearTextLocal = FText::FromString(YearStringLocal);
	return YearTextLocal;
}

/* Sets the numeric text for seconds on a clock */
FText UGameClockWidget::SetSecondText()
{
	//declare FString
	FString SecondStringLocal;

	//Get int for year local and set string
	int SecondsLocal = DateTimeStruct.GetSecond();
	SecondStringLocal = FString::FromInt(SecondsLocal);
	if (SecondsLocal < 10)
	{
		SecondStringLocal = "0" + SecondStringLocal;
	}

	//Set and return FString
	FText SecondTextLocal = FText::FromString(SecondStringLocal);
	return SecondTextLocal;
}

/* Sets numeric text for minutes on a clock */
FText UGameClockWidget::SetMinuteText()
{
	//declare FString
	FString MinuteStringLocal;

	//Get int for year local and set string
	int MinutesLocal = DateTimeStruct.GetMinute();
	MinuteStringLocal = FString::FromInt(MinutesLocal);
	if (MinutesLocal < 10)
	{
		MinuteStringLocal = "0" + MinuteStringLocal;
	}

	//Set and return FString
	FText MinuteTextLocal = FText::FromString(MinuteStringLocal);
	return MinuteTextLocal;
}

/* Sets Numeric value for hours on a clock - both 12 and 24 hour clock versions are available*/
FText UGameClockWidget::SetHourText()
{
	//declare FString
	FString HourStringLocal;

	//Get int for year local and set string
	int HourLocal;
	if (bUse12HourClock != true)
	{
		HourLocal = DateTimeStruct.GetHour(); // 24 hour clock version
	}
	else
	{
		HourLocal = DateTimeStruct.GetHour12(); // 12 hour clock version
	}
	HourStringLocal = FString::FromInt(HourLocal);
	//Set and return FString
	if (HourLocal < 10)
	{
		HourStringLocal = "0" + HourStringLocal;
		FText HourTextLocal = FText::FromString(HourStringLocal);
		return HourTextLocal;
	}
	else
	{
		
		FText HourTextLocal = FText::FromString(HourStringLocal);
		return HourTextLocal;
	}
}

/* Sets AM or PM text - do not use if using a 24 hour clock */
FText UGameClockWidget::SetTextAMPM()
{
	//Set FString
	if (bUse12HourClock == true) // aimed at preventing this from being set if 24 hour clock is being used.
	{
		if (DateTimeStruct.IsMorning())
			return FText::AsCultureInvariant("AM");
		else
			return FText::AsCultureInvariant("PM");
	}
	// return blank if 24 hour clock is being used.
	return FText();
}

/* Creates a symbol for reducing speed */
FText UGameClockWidget::SetSpeedReduceButtonText()
{
	return FText::AsCultureInvariant("<<");
}

/* Creates a symbol for play / pause */
FText UGameClockWidget::SetPlayPauseButton()
{
	if(!bIsPaused) // if not paused
		return FText::AsCultureInvariant("| |");
	else
		return FText::AsCultureInvariant(">");
}

/* Creates a symbol for increasing speed */
FText UGameClockWidget::SetSpeedIncraseButtonText()
{
	return FText::AsCultureInvariant(">>");
}

/* Event that passes the speed settings to player - uses interface to communicate to game state */
void UGameClockWidget::SetGameSpeed(int CurrentSpeedSetting)
{
	/*
	* Values used here are porportional.
	* Thus larger values = slower time dialation
	* Lower values = faster time dialation
	*/

	float SpeedMutliplerLocal = UGameTimeFunc::SetSpeedMultiplier(SpeedSetting);
	//switch (CurrentSpeedSetting)
	//{
	//case 0:
	//{
	//	SpeedMutliplerLocal = 0.0f;
	//	break;
	//}
	//case 1:
	//{
	//	SpeedMutliplerLocal = 1.75f;
	//	break;
	//}
	//case 2:
	//{
	//	SpeedMutliplerLocal = 1.25f;
	//	break;
	//}
	//case 3: //  also default
	//{
	//	SpeedMutliplerLocal = 1.0f;
	//	break;
	//}
	//case 4:
	//{
	//	SpeedMutliplerLocal = 0.5f;
	//	break;
	//}
	//case 5:
	//{
	//	SpeedMutliplerLocal = 0.25f;
	//	break;
	//}
	//default:
	//{
	//	SpeedMutliplerLocal = 1.0f;
	//	break;
	//}
	//}

	if (PlayerControllerRef->GetClass()->ImplementsInterface(UGameTimeInterface::StaticClass()))
	{
		IGameTimeInterface::Execute_UpdateGameSpeed(PlayerControllerRef, SpeedMutliplerLocal); // ZACH
	}
}

bool UGameClockWidget::SetGameSpeedInt_Implementation(float GameSpeed)
{
	SpeedSetting = UGameTimeFunc::GetGameSpeedInt(GameSpeed);
	// CHANGE THE DEFAULT HERE IF YOU CHANGE THE DEFAULT IN GAMESTATE
	//float DefaultGameSpeedLocal = 60.0f;

	//float SpeedSettingLocal = GameSpeed / DefaultGameSpeedLocal;
	//if (SpeedSettingLocal == 0.0f)
	//{
	//	SpeedSetting = 0;
	//}
	//else if (FMath::IsNearlyEqual(SpeedSettingLocal, 1.75f, 0.1f))
	//{
	//	SpeedSetting = 1;
	//}
	//else if (FMath::IsNearlyEqual(SpeedSettingLocal, 1.25f, 0.1f))
	//{
	//	SpeedSetting = 2;
	//}
	//else if (FMath::IsNearlyEqual(SpeedSettingLocal, 1.0f, 0.1f))
	//{
	//	SpeedSetting = 3;
	//}
	//else if (FMath::IsNearlyEqual(SpeedSettingLocal, 0.5f, 0.1f))
	//{
	//	SpeedSetting = 4;
	//}
	//else if (FMath::IsNearlyEqual(SpeedSettingLocal, 0.25f, 0.1f))
	//{
	//	SpeedSetting = 5;
	//}
	//else // defualt
	//{
	//	SpeedSetting = 3;
	//}
	SetGameTimeProgBar();
	CheckButtons();
	return true;
}

/* Player control over game speed; decrease - implemented via button press */
void UGameClockWidget::DecreaseSpeed()
{
	if (SpeedSetting >= 1) // Player cannot unpause by reducing speed
	{
		--SpeedSetting; // reduce speed stting
		SetGameSpeed(SpeedSetting);
		if (SpeedSetting == 0) // check if the player paused the game by reducing speed
		{
			StoredSpeedSetting = 1; // this was the last SpeedSetting before game paused
			bIsPaused = true;
		}
	}
}

/* Player control over game speed; increase - implemented via button press */
void UGameClockWidget::IncreaseSpeed()
{
	if (bIsPaused)
	{
		bIsPaused = false; // unapuse the game
		SpeedSetting = StoredSpeedSetting;
		SetGameSpeed(SpeedSetting);
	}
	else if (SpeedSetting < 5) // attempt to clamp at 5.
	{
		++SpeedSetting; // increase speed setting
		SetGameSpeed(SpeedSetting);
	}
	else if (SpeedSetting > 5) // should never happen
	{
		SpeedSetting = 5;
		SetGameSpeed(SpeedSetting);
	}
}

/* Method for pausing and unpausing the game */
void UGameClockWidget::PlayPauseGame()
{
	if (bIsPaused) // check if game is already paused
	{
		bIsPaused = false; // unapuse the game
		SpeedSetting = StoredSpeedSetting;
		SetGameSpeed(SpeedSetting);
	}
	else
	{
		StoredSpeedSetting = SpeedSetting; // Store current SpeedSetting when func (via button) is pressed
		SpeedSetting = 0; // Set speed setting to 0, will also disable DecreaseSpeed();
		bIsPaused = true; 
		SetGameSpeed(SpeedSetting);
	}
}

void UGameClockWidget::SetGameTimeProgBar()
{
	GameTimeProgBar->SetPercent((SpeedSetting / 5.0f));
}

bool UGameClockWidget::SetGameDateTime_Implementation(FDateTime TimeStruct)
{
	DateTimeStruct = TimeStruct;
	return true;
}

bool UGameClockWidget::SetGameDay_Implementation(int GameDay)
{
	Day = GameDay;
	return true;
}

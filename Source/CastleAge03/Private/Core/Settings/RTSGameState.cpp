// Code written by Zachary Parker for Feldfeste 
// written by Zachary J.Parker(2021), adapted from earlier work.
// Camera pawn code may be used for this project and has been previously used by the above in a tutorial


#include "Core/Settings/RTSGameState.h"
#include "Kismet/GamePlayStatics.h"
#include "Core/Player/RTSPlayerControllerBase.h"
#include "Core/Actors/SunRotationBase.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/World.h"

ARTSGameState::ARTSGameState()
{
	// init
	
	/*
	* All attributes not addressed here are declared and init in the header. 
	* This decision to do so in this class, is due to issues with the constructors having known bugs
	* In GameState, as it is the primary communication tool between host and clients, it was decided that
	* to help reduce chances of errors as much would be declared and init in the header.
	* 
	* While, for example, the CameraPawn is vital to gameplay, if there are any major issues with it
	* 1) they will be local and not game wide
	* 2) the code should correct itself (e.g., spring arm is off, the zoom function on tick will reset the springarm)
	*/

	/* The below commented out section
	* is for use in debugging and should not be used (as noted above)
	* for game play use
	* 
	// Game calendar and clock
	//Years = 1100;
	//Months = 10;
	//Days = 21;
	//DayOfWeek = 1;
	//
	//DayCounter = 0; // NEVER CHANGE

	//Hours = 13;
	//Minutes = 30;
	//Seconds = 12;

	//GameSpeed = 60;
	//DefaultGameSpeed = GameSpeed; 
	*/


	GameTime = (Hours + (Minutes / float(60.0f)) + (Seconds / float(3600.0f)));
	DateTimeStruct = (Years, Months, Days, Hours, Minutes, Seconds); 
	RoS_SetSeason(); // ensure the correct season is set

	// Tick (DO NOT CHANGE!)
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Replicated variables
void ARTSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSGameState, Years);
	DOREPLIFETIME(ARTSGameState, Months);
	DOREPLIFETIME(ARTSGameState, Days);
	DOREPLIFETIME(ARTSGameState, DayOfWeek);
	DOREPLIFETIME(ARTSGameState, DayCounter);
	DOREPLIFETIME(ARTSGameState, Hours);
	DOREPLIFETIME(ARTSGameState, Minutes);
	DOREPLIFETIME(ARTSGameState, Seconds);
	DOREPLIFETIME(ARTSGameState, Seasons);
	DOREPLIFETIME(ARTSGameState, GameSpeed);
	DOREPLIFETIME(ARTSGameState, DefaultGameSpeed);
	DOREPLIFETIME(ARTSGameState, GameTime);
	DOREPLIFETIME(ARTSGameState, DateTimeStruct);
}

// begin play
void ARTSGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		RoS_GetSunRotationActor();
		// The delay looks bad right now, please note that there will be a menu before the match starts so the issues with the delay will not be noticeable later.
		GetWorld()->GetTimerManager().SetTimer(BeginPlayTimerHandle, this, &ARTSGameState::RoS_DelayedCollectPlayerData, 2.5f, true);
		RoS_SetSeason(); // ensure the correct season is set
	}
}

void ARTSGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameSpeed != 0 && HasAuthority())
	{
		RoS_CalculateTime(DeltaTime, GameSpeed, GameTime);
		RoS_SetClockCalendar();
		RoS_PassDateTimeStruct();
		RoS_PassDayOfWeek();
	}
	if (HasAuthority())
	{
		RoS_PassGameSpeed();
	}
}

void ARTSGameState::RoS_DelayedCollectPlayerData_Implementation()
{
	// Get all players loaded into the game
	RoS_SetAllPlayerControllerRefs();
	// Get the host player, who should be at index 0 of player controllers.
	UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0);
	GetWorld()->GetTimerManager().ClearTimer(BeginPlayTimerHandle);
}

void ARTSGameState::RoS_SetAllPlayerControllerRefs_Implementation()
{
	TArray<AActor*> FoundActorsLocal;
	FoundActorsLocal.Empty(); // clear any junk
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARTSPlayerControllerBase::StaticClass(), FoundActorsLocal);
	int i = 0;
	for (AActor* TargetController : FoundActorsLocal)
	{
		ARTSPlayerControllerBase* TargetControllerLocal = Cast<ARTSPlayerControllerBase>(TargetController);
		if (TargetControllerLocal != nullptr)
		{
			PlayersRef.EmplaceAt(i, TargetControllerLocal);
			i++;
		}
	}
}

void ARTSGameState::RoS_CalculateTime_Implementation(float DeltaTime, float CurrentGameSpeed, float GameTimeIn)
{
	GameTime = fmod(((DeltaTime / CurrentGameSpeed) + GameTimeIn), 24.0f); // Make sure game time does not excede a 24 hour day
	DayCounter = (((DeltaTime / CurrentGameSpeed) + GameTimeIn) / 24); // when game time reachers 24 update this to 1. This will be 0 otherwise
	// GameTimeOut = GameTime; // for BP use only. Not possible with replication as returns are not allowed.
	// DayCounterOut = DayCounter; // For BP use only. Not possible with replication as returns are not allowed.
}

void ARTSGameState::RoS_SetClockCalendar_Implementation()
{
	// Set clock

	// Caulcate Seconds, Minutes, Hours
	float InitialConversionLocal = GameTime * 3600.0f; // Creates a local variable of game time in seconds.
	Seconds = floor(fmod(InitialConversionLocal, 60)); // 0 to 59, duuuh. (the duh is to myself) - take the remainder of initial divided by 60
	Minutes = floor(fmod(int(InitialConversionLocal / 60), 60.0f)); // take the whole number of initial divided by 60, then take the remainder of that value devided by 60
	Hours = floor(fmod(int((InitialConversionLocal / 60) / 60), 24.0f)); // repeat process for seconds and floor with the whole integer, then get the remainder of that value divided by 24

	// Set Calendar
	
	// Work out which day of the week (Sun - Sat) is it.
	DayOfWeek += DayCounter; // each time the Day Counter goes up add 1 (DayCounter is either 1 or 0) to DayOfWeek
	if (DayOfWeek > 7)
		DayOfWeek = 1; // the week has reset to Sunday

	// Calculate Day, Mont, Year
	Days += DayCounter; // Each time the Day Counter goes up, add 1 to Days (of the month)
	if (Days > FDateTime::DaysInMonth(Years, Months)) // if the new values of Days > Days in the Month, it is a new month
	{
		Days = 1;
		Months++; 
		RoS_SetSeason();
	}
	if (Months > 12) // if the month is 13, then well its January... restart the calendar
	{
		Months = 1;
		Years++;
		RoS_SetSeason(); // Ensure that a month ouside of the current scope is not passed in on year change
	}

	// Store the above
	RoS_SetDateTime(Years, Months, Days, Hours, Minutes, Seconds);
}

void ARTSGameState::RoS_SetDateTime_Implementation(const int& Year, const int& Month, const int& Day, const int& Hour, const int& Minute, const int& Second)
{
	DateTimeStruct = { Year, Month, Day, Hour, Minute, Second };
}

void ARTSGameState::RoS_PassDateTimeStruct_Implementation()
{
	// pass this to the player controllers
	for (ARTSPlayerControllerBase* ControllerLocal : PlayersRef)
	{
		if (ControllerLocal != nullptr)
		{
			ControllerLocal->GetClass()->ImplementsInterface(UGameTimeInterface::StaticClass());
			IGameTimeInterface::Execute_SetGameDateTime(ControllerLocal, DateTimeStruct);
/*
#if WITH_EDITOR
			FString TestLocal = ControllerLocal->GetName();
			UE_LOG(LogTemp, Warning, TEXT("Controller is %s"), *TestLocal);
#endif */
		}
		else
		{
			// this eles can be deleted. Was left here for debugging use only
#if WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("Controller is NOT valid"));
#endif
		}
	}

	// pass this for sun rotation
	if (SunActorRef != nullptr)
	{
		SunActorRef->GetClass()->ImplementsInterface(UGameTimeInterface::StaticClass());
		IGameTimeInterface::Execute_SetGameDateTime(SunActorRef, DateTimeStruct);
	}
	
}

void ARTSGameState::RoS_PassDayOfWeek_Implementation()
{
	for (ARTSPlayerControllerBase* ControllerLocal : PlayersRef)
	{
		if (ControllerLocal != nullptr)
		{
			if (ControllerLocal->GetClass()->ImplementsInterface(UGameTimeInterface::StaticClass()))
			{
				IGameTimeInterface::Execute_SetGameDay(ControllerLocal, DayOfWeek);
			}
		}
	}
}

void ARTSGameState::RoS_SetGameSpeed_Implementation(const float& SpeedMultiplier)
{
	GameSpeed = SpeedMultiplier * DefaultGameSpeed;
	GameSpeedControl.Broadcast(GameSpeed);
}

void ARTSGameState::RoS_PassGameSpeed_Implementation()
{
	for (ARTSPlayerControllerBase* ControllerLocal : PlayersRef)
	{
		if (ControllerLocal != nullptr)
		{
			if (ControllerLocal->GetClass()->ImplementsInterface(UGameTimeInterface::StaticClass()))
			{
				IGameTimeInterface::Execute_SetGameSpeedInt(ControllerLocal, GameSpeed);
			}
		}
	}
}

void ARTSGameState::RoS_GetSunRotationActor_Implementation()
{
	TArray<AActor*> FoundActorsLocal;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASunRotationBase::StaticClass(), FoundActorsLocal);
	for (AActor* TargetLocal : FoundActorsLocal)
	{
		// There should only ever be one item that is valid.
		if (TargetLocal != nullptr)
		{
			SunActorRef = TargetLocal;
		}
	}
}


void ARTSGameState::UpdateGameSpeed_Implementation(float SpeedMultiplier)
{
	if (HasAuthority())
	{
		RoS_SetGameSpeed(SpeedMultiplier);
	}
	else
	{
		RoS_SetGameSpeed(SpeedMultiplier);
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Called by non-authority"));
#endif
	}
}

ESeasons ARTSGameState::GetSeason_Implementation()
{
	return Seasons;
}

bool ARTSGameState::AddPlayerController_Implementation(APlayerController* PlayerControllerRef)
{
	if (HasAuthority())
	{
		if (PlayerControllerRef != nullptr)
		{
			ARTSPlayerControllerBase* TargetcontrollerLocal = Cast<ARTSPlayerControllerBase>(PlayerControllerRef);
			if (TargetcontrollerLocal != nullptr)
			{
				PlayersRef.AddUnique(TargetcontrollerLocal);
				return true;
			}
		}
	}
	else
	{
		return false;
	}
	return false;
}


void ARTSGameState::RoS_SetSeason_Implementation()
{
	if (Months == 12 || Months <= 2)
		Seasons = ESeasons::SeasonWinter;
	else if (Months >= 3 || Months <= 5)
		Seasons = ESeasons::SeasonSpring;
	else if (Months >= 6 || Months <= 8)
		Seasons = ESeasons::SeasonSummer;
	else if (Months >= 9 || Months <= 11)
		Seasons = ESeasons::SeasonFall;
}
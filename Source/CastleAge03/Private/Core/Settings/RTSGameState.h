// Code written by Zachary Parker for Feldfeste 
// written by Zachary J.Parker(2021), adapted from earlier work.
// Camera pawn code may be used for this project and has been previously used by the above in a tutorial

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Misc/DateTime.h"
#include "Net/UnrealNetwork.h"
#include "Core/Enums/CommonEnums.h"
#include "Core/Interfaces/GameTimeInterface.h"
#include "Core/Interfaces/GameStateInterface.h"
#include "TimerManager.h"
#include "RTSGameState.generated.h"

/* 
* This delegate is for all classes (BESIDES THE UMG!) that require to know what the current game speed is
* Declaration of multicast udelegate for updating game time for all instances outside of player controller/UMG
*/
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameTimeManager, float, GameTime);

class ARTSPlayerControllerBase;

/**
 * This code is the game state, replicated to all players
 * It houses the game time functions - including passing the game time variables to the UMG widgets (via interface)
 * In addition, it will pass the game time calculation out (via interface) for use by other classes (this may include an update where it uses the interface on itself - to be decided later)
 */

UCLASS()
class ARTSGameState : public AGameStateBase, public IGameTimeInterface, public IGameStateInterface
{
	GENERATED_BODY()

/************************************************
* CONSTRUCTORS
************************************************/
public:
	/* Default constructor */
	ARTSGameState();

/************************************************
* ATTRIBUTES
************************************************/
public:
	/* Multicast delegate instance. Called when game time is updated (on interface implementation) */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FGameTimeManager GameSpeedControl;

protected:
	/** Character controller for all players Refernece */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (AllowPrivateAccess = "true"))
	TArray<ARTSPlayerControllerBase*> PlayersRef;

	/** Character controller for the host players Refernece */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (AllowPrivateAccess = "true"))
	class ARTSPlayerControllerBase* HostPlayerControllerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (AllowPrivateAccess = "true"))
	class AActor* SunActorRef;
	
	// Change the values below (until the next comment) to change the default starting year, month, day, and time.
	// Do NOT change day counter (leave it at 0)
	// Days of week 1 = sunday, 7 = saturday (look up the correct day of week)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Calendar")
	int Years { 1100 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Calendar")
	int Months { 7 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Calendar")
	int Days { 21 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Calendar")
	int DayOfWeek { 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Calendar")
	int DayCounter { 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Clock")
	int Hours { 13 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Clock")
	int Minutes { 30 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Clock")
	int Seconds { 12 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Seeason")
	TEnumAsByte<ESeasons> Seasons;

	// Game Speed Settings
	/*
	* Real Time : GameSpeed = Time in game
	* 1s : 1 = 1 game hour
	* 1s : 60 = 1 game minute
	* 1s : 3600 = real time
	* 1m : 2.5 = 1 game day
	* 1hr : 150 = 1 game day
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Game Speed Settings")
	float GameSpeed { 60 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Game Speed Settings")
	float DefaultGameSpeed { 60 }; // maybe should be moved to a const float class global.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Game Speed Settings")
	float GameTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Game Speed Settings")
	FDateTime DateTimeStruct;

private:
	FTimerHandle BeginPlayTimerHandle;


/************************************************
* METHODS
************************************************/
protected:
	// Beginplay
	virtual void BeginPlay() override;

	// tick
	virtual void Tick(float DeltaTime) override;

	/************************************************
	* REPLICATED METHODS
	************************************************/
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "References")
	void RoS_DelayedCollectPlayerData();
	virtual void RoS_DelayedCollectPlayerData_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "References")
	void RoS_SetAllPlayerControllerRefs();
	virtual void RoS_SetAllPlayerControllerRefs_Implementation();

	/** Primary method for calculating game time passage - this function will be passed out using a variable*/
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Game Time")
	void RoS_CalculateTime(float DeltaTime, float CurrentGameSpeed, float GameTimeIn);
	virtual void RoS_CalculateTime_Implementation(float DeltaTime, float CurrentGameSpeed, float GameTimeIn);
	
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Game Time")
	void RoS_SetClockCalendar();
	virtual void RoS_SetClockCalendar_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Game Time")
	void RoS_SetDateTime(const int& Year, const int& Month, const int& Day, const int& Hour, const int& Minute, const int& Second);
	virtual void RoS_SetDateTime_Implementation(const int& Year, const int& Month, const int& Day, const int& Hour, const int& Minute, const int& Second);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Game Time")
	void RoS_SetSeason();
	virtual void RoS_SetSeason_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Interface")
	void RoS_PassDateTimeStruct();
	virtual void RoS_PassDateTimeStruct_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Interface")
	void RoS_PassDayOfWeek();
	virtual void RoS_PassDayOfWeek_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Game Time")
	void RoS_SetGameSpeed(const float& SpeedMultiplier);
	virtual void RoS_SetGameSpeed_Implementation(const float& SpeedMultiplier);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Interface")
	void RoS_PassGameSpeed();
	virtual void RoS_PassGameSpeed_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Sun Rotation")
	void RoS_GetSunRotationActor();
	virtual void RoS_GetSunRotationActor_Implementation();


	/************************************************
	* INTERFACE RELATED
	************************************************/
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
	void UpdateGameSpeed(float SpeedMultiplier);
	virtual void UpdateGameSpeed_Implementation(float SpeedMultiplier);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool AddPlayerController(APlayerController* PlayerControllerRef);
	virtual bool AddPlayerController_Implementation(APlayerController* PlayerControllerRef);

	/* Passes the season */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ESeasons GetSeason();
	virtual ESeasons GetSeason_Implementation();

public:
	/************************************************
	* GETTERS
	************************************************/
	/** Returns Game Time */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game Time")
	FORCEINLINE float GetGameTime() { return GameTime; }

	
};

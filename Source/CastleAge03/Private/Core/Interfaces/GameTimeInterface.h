// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/Enums/CommonEnums.h"
#include "GameTimeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameTimeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface allows for communication between:
 * GameState, GameClockWidget, MainWidget, PlayerController
 * 
 * Any other class that also needs GameTimeStruct might have this as well.
 * 
 * It will be implemented in both GameState and GameClockWidget to allow for communication back and forth
 * Look at comments below to know where each is implemented
 */
class IGameTimeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	 void UpdateGameSpeed(float SpeedMultiplier); // Will be implemented in GameState, called in GameClockWidget

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetGameDateTime(FDateTime TimeStruct); // Will be implemted in GameClockWidget, called in GameState.

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetGameDay(int GameDay); // Will be implemted in GameClockWidget, called in GameState.
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetGameSpeedInt(float GameSpeed); // will be implemented in widgets called in GameState

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ESeasons GetSeason(); // implemtned in resource actor, gets data from GameState.
};

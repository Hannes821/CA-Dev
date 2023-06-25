// Copyright Epic Games, Inc. All Rights Reserved.

#include "CastleAge03GameMode.h"
#include "CastleAge03PlayerController.h"
#include "CastleAge03Character.h"
#include "UObject/ConstructorHelpers.h"

ACastleAge03GameMode::ACastleAge03GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ACastleAge03PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Core/PrototypingFolder/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
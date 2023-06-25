// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RTSGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class URTSGameInstance : public UGameInstance
{
	GENERATED_BODY()

/************************************************
* CONSTRUCTORS
************************************************/
public:
	URTSGameInstance(const FObjectInitializer& ObjectInitializer);

/************************************************
* ATTRIBUTES
************************************************/

/************************************************
* METHODS
************************************************/
public:

	/************************************************
	* SETTERS
	************************************************/

	/* init */
	virtual void Init();

	/* CVAR for hosting a game */
	UFUNCTION(Exec)
	void Host();

	/* CVAR for joining a game */
	UFUNCTION(Exec)
	void Join(const FString& Address);
};

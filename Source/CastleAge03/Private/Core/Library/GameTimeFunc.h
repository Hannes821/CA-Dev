// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameTimeFunc.generated.h"

/**
 * This is mean to to serve as a function library.
 * Functions stored in here should be related to Game Time/Game Speed
 * All member methods must be static in nature.
 */

UCLASS(Abstract)
class UGameTimeFunc : public UObject
{
	GENERATED_BODY()

public:
	UGameTimeFunc();
	~UGameTimeFunc();

	UFUNCTION(BlueprintCallable)
	static int GetGameSpeedInt(float GameSpeed);


	UFUNCTION(BlueprintCallable)
	static float SetSpeedMultiplier(int CurrentSpeedSetting);

};

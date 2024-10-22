// Copyright Two Neurons Studio, LLC. Year of Publication 2024.

#pragma once

#include "CoreMinimal.h"
#include "MovementSettingsStructs.generated.h"

/* This struct is for user settings.
The value that is used here is the "current" value. 
The other values are used to clamp the current value when a user updates the values */
USTRUCT(BlueprintType)
struct FMovementSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Settings|Camera|Configuration")
	float Min			{ 0.f };
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Settings|Camera|Configuration")
	float Current		{ .5 };
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Settings|Camera|Configuration")
	float Max			{ 1.f };

	/* Default constructor - really shouldn't be used. */
	FMovementSettings()
	{
		Min = 0;
		Current = 5;
		Max = 10;
	}

	/* Constructor sets each value of the structure's members. */
	FMovementSettings(float MinIn, float CurrentIn, float MaxIn)
	{
		Min = MinIn;
		Current = CurrentIn;
		Max = MaxIn;
	}

	/* this constructor assumes a min of 0. */
	FMovementSettings(float CurrentIn, float MaxIn)
	{
		Min = 0;
		Current = CurrentIn;
		Max = MaxIn;
	}

	bool IsValid() const
	{
		return (Min < Max) && (Current >= Min) && (Current <= Max);
	}

	void FixToMin()
	{
		Current = Min;
	}

	void FixToMax()
	{
		Current = Max;
	}

	void FixToHalf()
	{
		if (Min == 1)
			Current = Max / 2;
		else
			Current = ((Max - Min) / 2) + Min;
	}
};
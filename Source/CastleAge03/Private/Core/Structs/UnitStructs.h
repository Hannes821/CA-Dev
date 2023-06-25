// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "UnitStructs.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FUnitProfile
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AController* OwnerRef;


};
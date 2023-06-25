// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FCommonStructs.generated.h"

/**
 *  Common structures used throughout
 */

 /* This is a struct for min and max values*/
USTRUCT(BlueprintType)
struct FMInMaxStruct : public FTableRowBase
{
	GENERATED_BODY()
	/* What is the max value*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxValue;

	/* What is the min value*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinValue;
};

USTRUCT(BlueprintType)
struct FMaxCurrentStruct : public FTableRowBase
{
	GENERATED_BODY()
	/* What is the max value*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxValue;

	/* What is the min value*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentValue;
};

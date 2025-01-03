// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputMapInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInputMapInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCEDRTSCAMERA_API IInputMapInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InputUpdates")
	void ForceRebuildInputs();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InputUpdates")
	bool AddOverrideInputs();
};

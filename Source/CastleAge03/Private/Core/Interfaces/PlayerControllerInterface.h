// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/Enums/InputModesEnum.h"
#include "PlayerControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IPlayerControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UBuildingManagerBase* GetBuildingManager();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EPrimaryActionModes GetActionMode();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetActionMode(EPrimaryActionModes Selection);
};

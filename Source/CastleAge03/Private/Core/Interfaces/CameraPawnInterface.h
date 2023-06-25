// Camera Pawn Interface, written by Zachary J. Parker (2021), adapted from earlier work.
// Camera pawn Interface code may be used for this project and has been previously used by the above in a tutorial.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
//#include "Core/Components/CameraMovementComponent.h"
#include "Components/ActorComponent.h"
#include "CameraPawnInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCameraPawnInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ICameraPawnInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CallSetArmlength(float Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CallSetArmRotation(float Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CallSetFieldOfView(float Value);
};

// Copyright Two Neurons Studio, LLC. Year of Publication 2024.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RTSMovementInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Category = "CameraRTS")
class URTSMovementInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCEDRTSCAMERA_API IRTSMovementInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallSetArmlength(float Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallSetArmRotation(float Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallSetFieldOfView(float Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallPanCamera(float Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallRotateCamera(float Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallResetRotation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallSetDoAdvanceCamerCollision(bool NewValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallGetClampedSpringArmLength(float& NewLengthOut);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|MovementSettings")
	void PushMovementSpeed(float NewSpeed);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|MovementSettings")
	void PushPanSensitivity(float NewPanSensitivity);
};

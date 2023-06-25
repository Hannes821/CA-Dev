// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/Enums/BuildingEnums.h"
#include "BuildingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBuildingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IBuildingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerDestroyPreview();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetRowName(FName TargetRowName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetOwner(APlayerController* TargetOwner);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetPreviewMaterial();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerMovePreview(FVector InNewLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerRotatePreview(FRotator NewRotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetConstructionStatus(EConstructionStatus Status);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerAddConstructedTime(float DeltaConstructionTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetMesh(int Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetBuildingStatus(int Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetBuildingMinStability(float NewStability);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetIsDestroyed(bool Value);
};

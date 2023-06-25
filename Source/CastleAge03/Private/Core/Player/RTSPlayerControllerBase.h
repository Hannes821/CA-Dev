// RTS Player controller, written by Zachary J. Parker (2021), adapted from earlier work.
// RTS player controller code may be used for this project and has been previously used by the above in a tutorial.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/Player/CameraPawn.h"
#include "Core/Enums/InputModesEnum.h"
#include "Core/Components/BuildingManagerBase.h"
#include "Core/Components/ResourceManagerBase.h"
#include "Core/Widgets/MainUIBase.h"
#include "Core/Interfaces/GameTimeInterface.h"
#include "Core/Interfaces/PlayerControllerInterface.h"

#include "Math/UnrealMathUtility.h"
#include "Core/Enums/ResourcesEnum.h"

#include "RTSPlayerControllerBase.generated.h"

/**
 * This is the primary player controller.
 */
UCLASS()
class ARTSPlayerControllerBase : public APlayerController, public IGameTimeInterface, public IPlayerControllerInterface
{
	GENERATED_BODY()

/************************************************
* CONSTRUCTORS
************************************************/
public:
	ARTSPlayerControllerBase();

/************************************************
* ATTRIBUTES
************************************************/
protected:
	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
	ACameraPawn* CameraPawnRef; // consider replacing with interface (I do not mind casting here as the controller should have a copy of the camera)

	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
	UCameraMovementComponent* CameraMovementRef;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
	UBuildingManagerBase* BuildingMangerRef;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
	UResourceManagerBase* ResourceManagerRef;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EPrimaryActionModes> ActionMode{ Selection };

	FVector MousePosition;

private:
	FTimerHandle DelayStartTimerHandle;

public:
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite)
	UMainUIBase* MainUI;

protected:
	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
	float DebugValueFloat{ 0.0f };

/************************************************
* METHODS
************************************************/
protected:
	// begin play
	virtual void BeginPlay() override;

public:
	// Tick
	virtual void Tick(float DeltaTime) override;

	// Bind functionality to input
	virtual void SetupInputComponent();

	virtual void DelayedBeginPlay();

	UFUNCTION(BlueprintCallable)
	virtual void SendIdentityToServer(APlayerController* SelfRef); // NOT USED

	virtual void CallMoveFoward(float Value);

	virtual void CallMoveRight(float Value);

	virtual void CallIncraseSpeedModifier();

	virtual void CallDecreaseSpeedModifier(); 

	virtual void CallZoomIn();

	virtual void CallZoomOut();

	virtual void CallZoomReset();

	virtual void CallPanReset();

	virtual void CallCameraReset();

	virtual void CallRotatePreviewLeft();

	virtual void CallRotatePreviewRight();

	virtual void CallEndRotatePreivew();

	UFUNCTION(BlueprintCallable)
	virtual void PrimaryActionSwitcher();

	/* Method for when */
	UFUNCTION(BlueprintCallable)
	virtual void ConstructionActions();


	FVector SetMousePosition(float TraceDistance);

	/************************************************
	* INTERFACE RELATED
	************************************************/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetGameDateTime(FDateTime TimeStruct); // prototype
	virtual bool SetGameDateTime_Implementation(FDateTime TimeStruct);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetGameDay(int GameDay); // prototype
	virtual bool SetGameDay_Implementation(int GameDay);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateGameSpeed(float SpeedMultiplier);
	virtual void UpdateGameSpeed_Implementation(float SpeedMultiplier);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetGameSpeedInt(float GameSpeed);
	bool SetGameSpeedInt_Implementation(float GameSpeed);

	/************************************************
	* REPLICATED METHODS
	************************************************/
	UFUNCTION(BlueprintCallable, client, Reliable, Category = "GameTimeWidget")
	void RoC_PassDateTimeStruct(FDateTime TimeStruct);
	void RoC_PassDateTimeStruct_Implementation(FDateTime TimeStruct);

	UFUNCTION(BlueprintCallable, client, Reliable, Category = "GameTimeWidget")
	void RoC_PassDayOfWeek(int GameDay);
	void RoC_PassDayOfWeek_Implementation(int GameDay);

	UFUNCTION(BlueprintCallable, client, Reliable, Category = "GameTimeWidget")
	void RoC_PassGameSpeed(float GameSpeed);
	void RoC_PassGameSpeed_Implementation(float GameSpeed);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "GameTimeWidget")
	void RoS_SetGameSpeed(float GameSpeed);
	void RoS_SetGameSpeed_Implementation(float GameSpeed);

	UFUNCTION(BlueprintCallable, Client, Reliable) // Reliable because it is that 1 in 1000 flook that can hurt the experiience
	void ROC_SetActionMode(EPrimaryActionModes Selection);
	void ROC_SetActionMode_Implementation(EPrimaryActionModes Selection);

	/************************************************
	* GETTERS
	************************************************/

	// Had to be be renamed due to the fact this function exists in AActor - look up if we can use it instead.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FVector GetCursorPosition() const { return MousePosition; }


	/************************************************
	* INTERFACE RELATED
	************************************************/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UBuildingManagerBase* GetBuildingManager();
	virtual UBuildingManagerBase* GetBuildingManager_Implementation();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetActionMode(EPrimaryActionModes Selection);
	virtual void TriggerSetActionMode_Implementation(EPrimaryActionModes Selection);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EPrimaryActionModes GetActionMode();
	EPrimaryActionModes GetActionMode_Implementation();


	UFUNCTION(BlueprintCallable)
	void TestSet(EResources Value);
};

// Custom Movement Component (for custom camera pawn), written by Zachary J. Parker (2021), adapted from earlier work.
// Custom Movement Component code may be used for this project and has been previously used by the above in a tutorial.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "Core/Player/CameraPawn.h"
#include "CameraMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCameraMovementComponent : public UActorComponent
{
	GENERATED_BODY()

/************************************************
* CONSTRUCTORS
************************************************/
public:	
	// Sets default values for this component's properties
	UCameraMovementComponent();


/************************************************
* ATTRIBUTES
************************************************/
public:
	/** Attached Pawn */
	//UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite)
	//class ACameraPawn* CameraOwner;

	UPROPERTY(BlueprintReadWrite)
	APlayerController* PlayerControllerRef; // This is a pointer (hard reference) but to a parent class

private:
	/** Is Movement disabled? */
	bool bDisableCameraMovement;

	/** Disables Edge Scroll for Panning */
	bool bDisableEdgeScroll;

	// Movement Speed Related

	//** How fast the camera moves */
	float DefaultMovementSpeed;

	/** Modifies The default movement speed */
	float MovementSpeedModifier; // 1 or 2.

	/** Spped During Edge Scroll at Corners */
	float CornerEdgeScrollSpeed;

	// Camera Zoom Related

	/** Delta for Zoom */
	float DeltaZoomAmount;

	/** Zoom setting (for curve) */
	float ZoomAmount;

/************************************************
* METHODS
************************************************/
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/************************************************
	* SETTERS
	************************************************/
	UFUNCTION(BlueprintCallable, Category = "Movement|Speed")
	float SetMovementSpeedModifier(float ModifierAmount);

	UFUNCTION(BlueprintCallable, Category = "Movement|Speed")
	float SetMovementSpeed(float MovementSpeedAdjustment);

	UFUNCTION(BlueprintCallable, Category = "Bools")
	bool SetCameraDiabled(bool bDisableCamera);

	UFUNCTION(BlueprintCallable, Category = "Bools")
	bool SetDisableEdgeScroll (bool bDisableScroll);

	//Utility
	UFUNCTION(BlueprintCallable, Category = "Movement|Utility")
	virtual void BasicMovementControl(float AxisValueX, float AxisValueY);

	UFUNCTION(BlueprintCallable, Category = "Movement|Utility")
	virtual void PanCamera(float RotationAmount);

	UFUNCTION(BlueprintCallable, Category = "Movement|Utility")
	virtual void ResetPan();

	UFUNCTION(BlueprintCallable, Category = "Movement|Utility")
	virtual void EdgeScroll();

	UFUNCTION(BlueprintCallable, Category = "Zoom|Utility")
	virtual void ZoomIn();

	UFUNCTION(BlueprintCallable, Category = "Zoom|Utility")
	virtual void ZoomOut();

	UFUNCTION(BlueprintCallable, Category = "Zoom|Utility")
	virtual void ResetZoom();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	virtual void CameraReset();

	/************************************************
	* GETTERS
	************************************************/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category ="Movement|Speed")
	FORCEINLINE float GetCurrentMovementSpeed() { return DefaultMovementSpeed; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement|Speed")
	FORCEINLINE float GetSpeedModifier() { return MovementSpeedModifier; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Bools")
	FORCEINLINE bool IsCameraDisabled() { return bDisableCameraMovement; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Bools")
	FORCEINLINE bool IsEdgeScrollDisabled() { return bDisableEdgeScroll; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement|Location")
	FVector GetCameraLocation();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement|Direction")
	FVector GetFaceDirection();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement|Direction")
	FRotator GetCameraRotation();
};

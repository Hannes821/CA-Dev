// Copyright Two Neurons Studio, LLC. Year of Publication 2024.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "RTSMovementComponent.generated.h"

UCLASS( ClassGroup=(CameraRTS), meta=(BlueprintSpawnableComponent) )
class ADVANCEDRTSCAMERA_API URTSMovementComponent : public UActorComponent
{
	GENERATED_BODY()

/************************************************
* ATTRIBUTES
************************************************/
private:

	/* The player controller class, used to get mouse location for edge scrolling */
	APlayerController* PlayerControllerRef;

	// Configuration Options

	/* Is the camera able to move? True = Camera cannot move */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bDisableCameraMovement{ false };

	/* If the camera is edge scrolling the player should not be able to be able to move camera. True = Camera cannot move while edge scrolling */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bDisableMovementDueToEdgeScroll{ false };

	/* Is the camera ever allowed to edge scroll? True = Camera cannot edge scroll */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bAlwaysDisableEdgeScroll{ false };

	/* Is the camera able to edge scroll? True = Camera cannot edge scroll */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bDynamicallyDisableEdgeScroll{ false };

	/* Is the camera able to pan/rotate? True = Camera cannot pan/rotate */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bDisablePanning{ false };

	/* Edge scroll speed is automatically calculated based on DefaultMovementSpeed. 
	If you'd like to override the edge scroll speed, mark this as true and set CornerEdgeScrollSpeed */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bOverrideCornerMovementSpeed{ false };

	/* Edge scroll speed is automatically calculated based on DefaultMovementSpeed. 
	If you'd like to override the edge scroll speed, mark this as true and set CornerEdgeScrollSpeed */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bOverrideEdgeScrollSpeed{ false };
	
	/* Disable edge scroll while panning */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bDisableEdgeScrollWhilePanning{ true };

	// State Settings

	/* The default rotation related to zoom */
	UPROPERTY(Category = "Settings|Camera|Camera", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FRotator DefaultRotation = FRotator(0.f, 0.f, 0.f);

	/* How fast the camera can move this is set by the pawn (see MovementSpeedSettings) */
	float DefaultMovementSpeed{ 15.f };

	/* The actual movement speed modifier. This is not exposed to BP as it is a run-time variable.
	This should NOT be edited unless you are certain of what you are doing. Edit the Default and Active versions. */
	float MovementSpeedModifier{ 1.f };

	/* This flags if Default or Active MovementSpeedModifier is in use.  This is not exposed to BP as it is a run-time variable. 
	Do NOT edit this value. */
	bool bMovementModifierActive{ false };

	/* Modifies the default movement speed. If set to 0, the camera will NOT move. Default value is 1.
	It is recommended that you do not change this value */
	UPROPERTY(Category = "Settings|Camera|Movement", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float DefaultMovementSpeedModifier{ 1.f };

	/* Modifies the default movement speed. If set to 0, the camera will NOT move when active. Default value is 2.
	It is recommended that you do go below the DefaultMovementSpeedModifier value. 
	It is recommended that you use a value that is double the DefaultMovementSpeedModifier */
	UPROPERTY(Category = "Settings|Camera|Movement", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float ActiveMovementSpeedModifier{ 2.f };

	/* Speed at which the camera moves when the pawn is moving on BOTH (and only when both) X and Y planes.
	By default this value is calculated on beginplay using the DefaultMovementSpeed. 
	Changing this value only has an effect IF bOverrideCornerMovementSpeed is marked true.*/
	UPROPERTY(Category = "Settings|Camera|Movement", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float CornerMovementSpeed{ 10.606601715f };

	/* Speed at which the camera moves when edge scrolling. By default this value is calcualted on beginplay.
	It calculates movement for when the pawn is moving on BOTH (and only when both) X and Y planes.
	Changing this value only has an effect IF bOverrideEdgeScrollSpeed is marked true.
	By default this is calculated using DefaultMovementSpeed. */
	UPROPERTY(Category = "Settings|Camera|Movement", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float CornerEdgeScrollSpeed{ 10.606601715f };

	/* Porportion of screen 'covered' before edge scroll starts. See documentation for a visual explanation */
	UPROPERTY(Category = "Settings|Camera|Movement", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float EdgeScrollTriggerValue{ .025 };

	/* how far down to the bottom of the screen or how far to the right of the screen the mouse is before edge scroll starts.
	This is not exposed as the user enters a single value that calculates this value (1 -  )*/
	float EdgeScrollHighValue{ .975 };
	/* How far up to the top of the screen or how far to the left of thee screen the mouse is before edge scroll starts. */
	float EdgeScrollLowValue{ .025 };

	/* How much does the zoom change each time zoom in or zoom out is called. */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float DeltaZoom{ 0.1f };

	/* Does the camera zoom in/out fully based on a single click?. (default: false) */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bSingleClickZoom{ false };
	/* How fast does the camera zoom in/out on a single click zoom (only ative if bSingeClickZoom is marked true) */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float SingleClickZoomSpeed{ 0.001 };
	/* unexposed variable, the timer used to control single click zoom in*/
	FTimerHandle SingleClickZoomInTimerHandle;
	/* unexposed variable, the timer used to control single click zoom out*/
	FTimerHandle SingleClickZoomOutTimerHandle;

	/* How big of a change in from current zoom to new zoom is expected. 
	This and DefaultZoomAmount should have the same value at start. 
	This value will override DefaultZoomAmount on BeginPlay */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float ZoomAmount{ 0.5f };

	/* How big of a change in from current zoom to new zoom is expected. 
	This and ZoomAmount should have the same value at start.*/
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float DefaultZoomAmount{ 0.5f };

	/* The amount the camera is zoomed in, ranging 0 to 1. Where 1 is fully zoomed in and 0 is fully zoomed out. 
	Default is half-way between at .5. This is not exposed to BP as it is a run-time variable. 
	Do NOT edit this value. */
	float Zoom{ 0.5f };

	/* This is a checker boolean that ensure that the interface for the pawn is connected. */
	bool bMovementInterfaceConnected{ false };

	

/************************************************
* CONSTRUCTORS
************************************************/
public:	
	// Sets default values for this component's properties
	URTSMovementComponent();

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
	* Setup
	************************************************/
	/* Override default movement speed. Calls both CalculateCornerMovementSpeed && CalculateCornerEdgeScrollSpeed*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void SetDefaultMovementSpeed(float NewSpeed);
	void SetDefaultMovementSpeed_Implementation(float NewSpeed);
	/* Initializes MovementSpeedModifier to user-inputed DefaultMovementSpeedModifier. 
	This version uses the default and does not override. It is called on BeginPlay */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void UpdateMovementSpeedModifier();
	void UpdateMovementSpeedModifier_Implementation();
	/* Initializes MovementSpeedModifier to user-inputed DefaultMovementSpeedModifier.
	This overrides the movement speed modifier and should be used as needed at run-time */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void SetMovementSpeedModifier(float NewModifier);
	void SetMovementSpeedModifier_Implementation(float NewModifier);
	/* Calculates the default corner movement speed IF bOverrideCornerMovementSpeed is false*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CalculateCornerMovementSpeed();
	void CalculateCornerMovementSpeed_Implementation();
	/* Calculates the default edge scrool corner movement speed IF bOverrideEdgeScrollSpeed is false*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CalculateCornerEdgeScrollSpeed();
	void CalculateCornerEdgeScrollSpeed_Implementation();
	/* Calculates and sets the trigger areas for edge scroll */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void SetEdgeScrollTriggers();
	void SetEdgeScrollTriggers_Implementation();
	/* Sets the current zoom */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void SetZoomAmount();
	void SetZoomAmount_Implementation();


	/************************************************
	* Run-Time Events (Setters & Triggers)
	************************************************/
	/* This switches between the default and active MovementSpeedModifiers. 
	This version is to be pressed to active and presed again to deactive. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Setters")
	void TriggerMovementSpeedModifier();
	void TriggerMovementSpeedModifier_Implementation();
	/* This switches between the default and active MovementSpeedModifiers.
	This version is meant for press and release */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Setters")
	void ToggleMovementSpeedModifier(bool ModifyMovement);
	void ToggleMovementSpeedModifier_Implementation(bool ModifyMovement);

	/* Set if the camera movement is disable or not */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Setters")
	void SetCameraDisabled(bool DisableCamera);
	void SetCameraDisabled_Implementation(bool DisableCamera);
	/* Set if the camera movement is disable or not due to edge scroll */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Setters")
	void SetCameraDisabledDueToEdgeScroll(bool DisableCamera);
	void SetCameraDisabledDueToEdgeScroll_Implementation(bool DisableCamera);
	
	/* Set if the edge scroll is disabled or not. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Setters")
	void SetDisableEdgeScroll(bool DisableEdgeScroll);
	void SetDisableEdgeScroll_Implementation(bool DisableEdgeScroll);

	/* Set if panning is disabled or not */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Setters")
	void SetDisablePanning(bool DisablePanning);
	void SetDisablePanning_Implementation(bool DisablePanning);

	/************************************************
	* Run-Time Events (Movement)
	************************************************/
	/* This controls movement in the X and Y axes and acocunt for corner movement speed */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void BasicMovementControl(float AxisValueX, float AxisValueY);
	void BasicMovementControl_Implementation(float AxisValueX, float AxisValueY);

	/* This controls movement in the X axis (relative to the camera direction). It does not acocunt for corner movement speeds */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void MoveForward(FVector MovementX);
	void MoveForward_Implementation(FVector MovementX);
	/* This controls movement in the Y axis (relative to the camera direction). It does not acocunt for corner movement speeds */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void MoveRight(FVector MovementY);
	void MoveRight_Implementation(FVector MovementY);
	/* Do not use as a standa alone method. This accounts for movements in both axes but takes raw values so it does not account for corner movement speed. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void MoveForwardRight(FVector MovementX, FVector MovementY);
	void MoveForwardRight_Implementation(FVector MovementX, FVector MovementY);

	/* Edge scroll event, accounts for edge scroll corner movement speeds */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void EdgeScroll();
	void EdgeScroll_Implementation();

	/* Pans the camera left and right. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void PanCamera(float DeltaRotation);
	void PanCamera_Implementation(float DeltaRotation);
	/* Pans the camera up and down */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void RotateCamera(float DeltaRotation);
	void RotateCamera_Implementation(float DeltaRotation);
	/* Resets the camera rotation back to default */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void ResetPan();
	void ResetPan_Implementation();

	/* Passes to pawn, on tick, relevent  zoom information */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void ZoomEvents();
	void ZoomEvents_Implementation();
	/* Updates zoom amount for zooming in */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void ZoomIn();
	void ZoomIn_Implementation();
	/* Updates zoom amount for zooming out */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void ZoomOut();
	void ZoomOut_Implementation();
	/* Resets zoom amount to default zoom */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void ResetZoom();
	void ResetZoom_Implementation();

	/* Resets both zoom and pan*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void ResetCamera();
	void ResetCamera_Implementation();

	/* Single click zoom in event */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void SingleClickZoomIn();
	void SingleClickZoomIn_Implementation();
	/* Single click zoom out event */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement|Utility")
	void SingleClickZoomOut();
	void SingleClickZoomOut_Implementation();

	/************************************************
	* Getters
	************************************************/
	/* Return is the camera is disabled or not */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Getters")
	FORCEINLINE bool IsCameraDisabled() const { return bDisableCameraMovement; }

	/* Return is the camera is disabled due to edge scroll or not */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Getters")
	FORCEINLINE bool IsDisableDueToEdgeScroll() const { return bDisableMovementDueToEdgeScroll; }

	/* Returns the current (default) movement speed */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Getters")
	FORCEINLINE float GetCurrentMovementSpeed() const { return DefaultMovementSpeed; }

	/* Returns the current movement speed modifier */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Getters")
	FORCEINLINE float GetSpeedModifier() const { return MovementSpeedModifier; }

	/* Returns current corner movement speed */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Getters")
	FORCEINLINE float GetCornerMovementSpeed() const { return CornerMovementSpeed; }

	/* Returns current edge scroll corner movement speed*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Getters")
	FORCEINLINE float GetEdgeScrollCornerMovementSpeed() const { return CornerEdgeScrollSpeed; }

	/* Returns if panning is disabled */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Getters")
	FORCEINLINE bool IsPanningDisabled() const {return bDisablePanning; }

	/* Returns pawn's current rotation */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Getters")
	FORCEINLINE FRotator GetCameraRotation() const { return GetOwner()->GetActorRotation(); }

	/* Returns if edge scroll is disabled */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Getters")
	FORCEINLINE bool IsEdgeScrollDisabled() const { return bDynamicallyDisableEdgeScroll; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Getters")
	FORCEINLINE bool IsEdgeScrollAlwaysDisabled() const { return bAlwaysDisableEdgeScroll; }
};

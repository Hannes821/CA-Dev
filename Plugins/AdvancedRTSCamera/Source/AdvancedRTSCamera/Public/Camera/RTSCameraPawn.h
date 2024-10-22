// Copyright Two Neurons Studio, LLC. Year of Publication 2024.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "RTSSpringArm.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "RTSMovementComponent.h"
#include "../Interfaces/RTSMovementInterface.h"
#include "MovementSettingsStructs.h"
#include "RTSCameraPawn.generated.h"

// Main RTS pawn class
UCLASS(ClassGroup = (CameraRTS), Abstract)
class ADVANCEDRTSCAMERA_API ARTSCameraPawn : public APawn, public IRTSMovementInterface
{
	GENERATED_BODY()

/************************************************
* ATTRIBUTES
************************************************/

	/*************
	* Components
	*************/

protected: 
	/* Custom movement component for player camera */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "PlayerControls", meta = (AllowPrivateAcces = "true"))
	TObjectPtr<URTSMovementComponent> PawnMovementComponent;
private:
	/* Collision Sphere and root component of camera */
	UPROPERTY(Category = "Settings|Pawn|Components", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> CollisionSphere; 
	/* Camera springarm/Boom, used for zoom and for lag */
	UPROPERTY(Category = "Settings|Pawn|Components", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URTSSpringArm> CameraSpringArm;
	/* Player Camera */
	UPROPERTY(Category = "Settings|Pawn|Components", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> PlayerCamera;

	/* The player controller class, used to get camera tracking */
	TObjectPtr<APlayerController> PlayerControllerRef;



	/* Default spring arm length. Note that using curves will override this and should be set in the curve menu */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float DefaultZoomDistance{ 1350.0f };
	/* Default Pawn Rotation. Note that using curves will override this.  */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FRotator DefaultCameraRotation { -75.0f, 0.0f, 0.0f };
	/* Default Camera Field of View. Note that using curves will override this*/
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float DefaultFieldOfView{ 90.0f };

	/*************
	* Configuration
	*************/
	
	/* Can the camera zoom in and out? If marked false, no zoom events run. (default: true) */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bZoom{ true };
	/* You can use float curves or the built in EaseInOut Functions. */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bUseCurves{ true };
	/* On zoom does the spring arm length change?*/
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bZoomSpringArm{ true };
	/* On zoom does the rotation of the camera change?*/
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bZoomRotation{ true };
	/* If you want the player to manually rotate the camera up and down (this will be added into panning)
	(should NOT be used if bZoomRotation is true) */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bRotateOnPan{ false };
	/* On zoom does the camera field of view change?*/
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bZoomFOV{ true };
	/* If the pawn's distance from the ground should be changed or not (default is true) */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bAdjustPawnHeightForTerrain{ true };
	/* If the pawn should fade materials when near by (default is true) */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bRunFadeCheck{ true };
	/* If the pawn can track objects/actors in the world (default is true) */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanTrack{ true };
	/* Does the camera use a smoothed collision check */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bDoAdvanceCollisionTest{ true };
	/* Should this camera already start attached to a trackable object (make sure all default tracking settings are included; default is false).
	You will also need to set StartingFollowTarget */
	UPROPERTY(Category = "Settings|Camera|Configuration", EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bStartAttachedToTrack{ false };
	/* The context mapping input - fill this out if you want to remap inputs*/
	UPROPERTY(Category = "Settings|Camera|Configuration", BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UPlayerMappableInputConfig* PMI;

	/*************
	* Movement
	*************/

	/* How fast can the camera move */
	UPROPERTY(Category = "Settings|Camera|Movement", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FMovementSettings MovementSpeedSettings{ 10.f, 15.f, 20.f };
	/* Set by the MovementSpeedSettigns BEFORE loading of updated values. This variable is used to restore the origianal settings if an updated value is applied */
	float DefaultMovementSpeed{ 15.f };

	/*************
	* Zoom
	*************/

	/* Curve for CameraSpringArm */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> CurveZoomArmLength;
	/* Curve for Zoom Rotation */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> CurveZoomRotation;
	/* Curve for Zoom Field of View */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> CurveZoomFOV;

	/* How short is the spring arm is when zoomed in (to use mark bUseCurves as FALSE) */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float SpringArmZoomIn{ 300.f };
	/* How long is the spring arm is when zoomed in (to use mark bUseCurves as FALSE) */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float SpringArmZoomOut{ 1350.f };
	/* How much the spring arm length changes if zooming in versus out. (to use mark bUseCurves as FALSE) */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float SpringArmZoomStep{ 100.f };

	/* How close to the ground (closer to 0) is the camera when zoomed in (to use mark bUseCurves as FALSE) */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float PitchZoomIn{ -25.f };
	/* How far to the ground (farther to 0) is the camera when zoomed in (to use mark bUseCurves as FALSE) */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float PitchZoomOut{ -45.f };
	/* How much the rotation changes if zooming in versus out. (to use mark bUseCurves as FALSE) */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float PitchZoomStep{ 5.f };
	
	/* What is the FOV when zoomed in (to use mark bUseCurves as FALSE) */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float FieldOfViewZoomIn{ 90.f };
	/* What is the FOV when zoomed in (to use mark bUseCurves as FALSE) */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float FieldOfViewZoomOut{ 45.f };
	/* How much the FOV hanges if zooming in versus out. (to use mark bUseCurves as FALSE) */
	UPROPERTY(Category = "Settings|Camera|Zoom", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float FieldOfViewZoomStep{ 12.f };

	/* This is used for smoothing behind the sense and should not be edited. Default value should be the same as the default zoom level (.5) */
	float TimeLastFrame{ 0.5f };
	/* This is used for smoothing behind the sense and should not be edited */
	float ZoomAlpha{ 0.f };

	/* Clamps the spring arm based on advanced collision test */
	float ClampedSpringArmMax{ 3500 };

	/*************
	* Panning
	*************/

	/* How much to pan the camera for a set amount (this should be a divisor of 360)*/
	UPROPERTY(Category = "Settings|Camera|Panning", BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float SetPanAmount{ 15.f };
	/* How fast to pan the mouse based on mouse movement */
	UPROPERTY(Category = "Settings|Camera|Panning", BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FMovementSettings PanSpeedSettings { 1.f, 5.f, 10.f };
	// unexposed while this value is used, it is set by the structure above.
	float PanSpeed{ 5.f };
	// Unexposed, this variable is used to restore defaults with the menu system
	float DefaultPanSpeed{ 5.f };
	/* How much do you allow the camera to move up and down (this is used as part of panning when zoom does not affect rotation) */
	UPROPERTY(Category = "Settings|Camera|Panning", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float MinRotation{ -45.f };
	/* How much do you allow the camera to move up and down (this is used as part of panning when zoom does not affect rotation)*/
	UPROPERTY(Category = "Settings|Camera|Panning", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float MaxRotation{ -25.f };


	/*************
	* Height Adjustment
	*************/

	/* When adjust for height should this be a trace complex? (default: false) */
	UPROPERTY(Category = "Settings|Camera|HeightAdjustment", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bHeightAdjustTraceComplex{ false };
	/* Adjusts the target height based on landscape's Z coordinate (defined in LandscapeZ). Useful if landscape is not 0 on Z-translation. */
	UPROPERTY(Category = "Settings|Camera|HeightAdjustment", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bUseLandscapeOffset{ false };
	/* How much to offset the target height based on landscape (only matters if bUseLandscapeOffset is marked true)*/
	UPROPERTY(Category = "Settings|Camera|HeightAdjustment", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float LandscapeZ{ 100.f };
	/* Add any actors you want to ignore while adjusting camera height. The pawn is included already. */
	UPROPERTY(Category = "Settings|Camera|HeightAdjustment", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<AActor>> ActorsToIgnore;
	/* What channel are you using to determine if the pawn height should adjust? (Useful for adding in custom channels). */
	UPROPERTY(Category = "Settings|Camera|HeightAdjustment", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> CameraHeightChannel{ ECollisionChannel::ECC_WorldStatic };
	/* What is the target height the pawn should be at? */
	UPROPERTY(Category = "Settings|Camera|HeightAdjustment", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float DesiredCameraHeight{ 189.f };
	/* What is the tolerance around that height? (Note, low values (<1.0) may result in jittering */
	UPROPERTY(Category = "Settings|Camera|HeightAdjustment", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float CameraHeightTolerance{ 5.f };
	/* Adjust the speed at which the camera height adjust */
	UPROPERTY(Category = "Settings|Camera|HeightAdjustment", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float CameraHeightAdjustExp{ 0.8f };

	/* The actual target height. */
	float TargetCameraHeight{ 189.f };
	/* Unexposed variable use for line traces. 
	The user sets values in this by updating the HeightAdjustment variables. */
	FCollisionQueryParams CameraHeightCollisonParams;
	/* Unexposed Variable used for calculating the alpha on lerping the camera position. This should never be edited. */
	float AdjustCameraAlpha{ 0.f };



	/*************
	* Object Tracking
	*************/

	/* What objects do you want to track/move to when clicked on*/
	UPROPERTY(Category = "Settings|Camera|ObjectTracking", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToTrack;
	/* How fast does the camera move to the tracked object? */
	UPROPERTY(Category = "Settings|Camera|ObjectTracking", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float TrackAttachSpeed{ .1 };
	/* Unexposed variable that determines of the pawn is currently tracking an object. */
	bool bIsTracking{ false };
	/* Unexposed variable that says if the pawn is currently moving to a tracked target */
	bool bIsAttaching{ false };
	/* Unexposed variable for the tracked target */
	TObjectPtr<AActor> FollowTargetRef{ nullptr }; 
	/* The actor to track when the camera starts attached to a trackable actor */
	UPROPERTY(Category = "Settings|Camera|ObjectTracking", EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> StartingFollowTarget{ nullptr };
	/* Unexposed variable for timer handle that runs the object tracking loop/ */
	FTimerHandle CameraAttachTimerHandle;
	/* Unexposed variable used for lerping form current location to track target location. */
	float AttachAlpha{ 0.f };

	/*************
	* Material Fade
	*************/
	
	/* What channel are you using to determine if an object's material should be faded? (Useful for adding in custom channels). */
	UPROPERTY(Category = "Settings|Camera|MaterialFade", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> CameraFadeChannel{ ECollisionChannel::ECC_WorldDynamic };
	/* How close an object has to be for fade material to be applied (default 500. Objects within 500UU will be faded).*/
	UPROPERTY(Category = "Settings|Camera|MaterialFade", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float FadeTraceDistance{ 500.f };
	/* User defined material used for when fading (this should be a translucent material of some sort) */
	UPROPERTY(Category = "Settings|Camera|MaterialFade", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInterface> FadeMaterial;

	/* Unexposed variable use for line traces when fading a material. */
	FCollisionQueryParams CameraFadeCollisonParams;
	/* Unexposed variable for the faded object's original materials. This should never be edited */
	TArray<UMaterialInterface*> OriginalMaterials;
	/* Unexposed variable Object that is being faded */
	TObjectPtr<AActor> FadeTarget{ nullptr };
	/* Unexposed variable that says if the pawn has a fade target or not */
	bool bHasFadeTarget{ false };
	/* Unexposed variable for the mesh of the targetted actor - this is the mesh that will be faded. */
	UStaticMeshComponent* FadeMesh{ nullptr };




/************************************************
* CONSTRUCTORS
************************************************/
public:
	// Sets default values for this pawn's properties
	ARTSCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/* A collection of RTS pawn specific tick events. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Tick")
	void TickEvents(float DeltaTime);
	void TickEvents_Implementation(float DeltaTime);


	/* Raises or lowers the height of the pawn based on track object (typically landscape). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Height")
	void AdjustCameraHeight(float DeltaTime);
	void AdjustCameraHeight_Implementation(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Height")
	void AdjustCameraSpringArmLength();
	void AdjustCameraSpringArmLength_Implementation();

	/* Runs check to see if an object should be faded */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Fade")
	void FadeEvent();
	void FadeEvent_Implementation();
	/* Sets the actor that should be faded */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Fade")
	void SetFadeTarget(AActor* Target = nullptr);
	void SetFadeTarget_Implementation(AActor* Target = nullptr);
	/* Set fade targets materials - by default this fades the target but also calls restore material. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Fade")
	void RunFadeTarget();
	void RunFadeTarget_Implementation();
	/* Restore the default material of the target object. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Fade")
	void RestoreTargetMaterials();
	void RestoreTargetMaterials_Implementation();


	/* Set the spring arm length during zoom events. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Zoom")
	void SetArmLength(float InZoomAmount);
	void SetArmLength_Implementation(float InZoomAmount);
	/* Sets the pawn rotation during zoom events. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Zoom")
	void SetArmRotation(float InZoomAmount);
	void SetArmRotation_Implementation(float InZoomAmount);
	/* Sets the camera FOV during zoom events. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Zoom")
	void SetFieldOfView(float InZoomAmount);
	void SetFieldOfView_Implementation(float InZoomAmount);


	/* Set if the pawn's height should be adjusted for terrain tracking. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Height")
	void SetShouldAdjustHeight(bool ShouldAdjustPawnHeight);
	void SetShouldAdjustHeight_Implementation(bool ShouldAdjustPawnHeight);
	/* Set if using landscape offset on z-axis. Automatically adjusts target height. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Height")
	void SetUseLandscapeOffset(bool UseOffset);
	void SetUseLandscapeOffset_Implementation(bool UseOffset);
	/* Set the landscape offset. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Height")
	void SetLandscapeOffset(float Offset);
	void SetLandscapeOffset_Implementation(float Offset);
	/* Set landscape offset on z-axis. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Height")
	void SetTargetHeight(float Target);
	void SetTargetHeight_Implementation(float Target);
	/* Set tolerance (kappa) for accepting pawn location. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Height")
	void SetCameraHeightTolerance(float Kappa);
	void SetCameraHeightTolerance_Implementation(float Kappa);
	/* Helper function that just sets variables */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Height")
	void AdjustDesiredHeightToTarget();
	void AdjustDesiredHeightToTarget_Implementation();

	/* Set if the pawn can fade materials of objects in range. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Fade")
	void SetCanFadeObjects(bool CanFade);
	void SetCanFadeObjects_Implementation(bool CanFade);
	/* Sets new material for fading actors in front of camera */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Fade")
	void SetFadeMaterial(UMaterialInterface* NewMaterial);
	void SetFadeMaterial_Implementation(UMaterialInterface* NewMaterial);
	/* Sets trance distance for fade */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Fade")
	void SetFadeTraceDistance(float Distance);
	void SetFadeTraceDistance_Implementation(float Distance);

	/* Sets if the pawn can zoom or not */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Zoom")
	void SetCanZoom(bool CanZoom);
	void SetCanZoom_Implementation(bool CanZoom);
	/* Sets if the pawn uses curves for zoom or not. Returns false if the curves are not valid.
	The return is only for error checking curves (it does NOT specify if a curve is being used or not) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Zoom")
	bool SetUsesCurves(bool UsesCurves);
	bool SetUsesCurves_Implementation(bool UsesCurves);
	/* Sets if the zoom changes spring arm */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Zoom")
	void SetZoomAdjustsSpringArm(bool AdjustSpringArm);
	void SetZoomAdjustsSpringArm_Implementation(bool AdjustSpringArm);
	/* Sets if the zoom changes rotation. If true it automatically sets bRotateOnPan to false (does not restore bRotateOnPan). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Zoom")
	void SetZoomAdjustsRotation(bool AdjustRotation);
	void SetZoomAdjustsRotation_Implementation(bool AdjustRotation);
	/* Sets if the zoom changes FOV */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Zoom")
	void SetZoomAdjustsFOV(bool AdjustFOV);
	void SetZoomAdjustsFOV_Implementation(bool AdjustFOV);

	/* Sets if the pan changes rotation. If true it automatically sets bZoomRotation to false (does not restore bZoomRotation). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Panning")
	void SetPanAdjustsRotation(bool AdjustRotation);
	void SetPanAdjustsRotation_Implementation(bool AdjustRotation);

	/* This updates PanSpeed using PanSpeedSettings.Current */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Panning")
	void SetPanSpeed();
	void SetPanSpeed_Implementation();
	/* this updates both PanSpeed and PanSpeedSettings.Current*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Panning")
	void UpdatePanSpeed(float NewSpeed);
	void UpdatePanSpeed_Implementation(float NewSpeed);
	
	/* This updates PanSpeed using PanSpeedSettings.Current */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void SetMovementSpeed();
	void SetMovementSpeed_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void UpdateMovementSpeed(float NewSpeed);
	void UpdateMovementSpeed_Implementation(float NewSpeed);


public:
	/* returns if the pawn is adjusting height based on terrain */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Height|Getters")
	FORCEINLINE bool GetAdjustHeightForTerrain() const { return bAdjustPawnHeightForTerrain; }
	/* returns if the adjusting height is using an offset  */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Height|Getters")
	FORCEINLINE bool GetUsesLandscapeOffset() const { return bUseLandscapeOffset; }
	/* returns the heigh adjustment offset (just because there is a value does not mean it is active) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Height|Getters")
	FORCEINLINE float GetLandscapeOffset() const { return LandscapeZ; }
	/* returns the user entered target height for pawn */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Height|Getters")
	FORCEINLINE float GetDesiredCameraHeight() const { return DesiredCameraHeight; }
	/* returns the adjusted and true target height for pawn */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Height|Getters")
	FORCEINLINE float GetTargetCameraHeight() const { return TargetCameraHeight; }
	/* returns the adjustment acceptance tolerance */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Height|Getters")
	FORCEINLINE float GetCameraHeightTolerance() const { return CameraHeightTolerance; }

	/* returns if pawn run fade check */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Fade|Getters")
	FORCEINLINE bool GetRunFadeCheck() const {return bRunFadeCheck; }
	/* returns if fade target's materials */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Fade|Getters")
	FORCEINLINE TArray<UMaterialInterface*> GetOriginalMaterials() const { return OriginalMaterials; }
	/* returns fade target */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Fade|Getters")
	FORCEINLINE AActor* GetFadeTarget() const { return FadeTarget; }
	/* returns if pawn has fade target */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Fade|Getters")
	FORCEINLINE bool GetHasFadeTarget() const { return bHasFadeTarget; }
	/* returns returns fade target's mesh */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Fade|Getters")
	FORCEINLINE UStaticMeshComponent* GetFadeMesh() const { return FadeMesh; }
	/* returns if fade material */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Fade|Getters")
	FORCEINLINE UMaterialInterface* GetFadeMaterial() const { return FadeMaterial; }
	/* returns if fade distance */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Fade|Getters")
	FORCEINLINE float GetFadeDistance() const { return FadeTraceDistance; }

	/* returns if the camera can zoom */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Zoom|Getters")
	FORCEINLINE bool GetCanZoom() const { return bZoom; }
	/* returns if the camera uses curves for zoom */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Zoom|Getters")
	FORCEINLINE bool GetIfUsesCurves() const { return bUseCurves; }
	/* returns if the camera adjusts spring arm on zoom */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Zoom|Getters")
	FORCEINLINE bool GetIfZoomAdjustsSpringArm() const { return bZoomSpringArm; }
	/* returns if the camera adjusts rotation  on zoom */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Zoom|Getters")
	FORCEINLINE bool GetIfZoomAdjustsRotation() const { return bZoomRotation; }
	/* returns if the camera adjusts FOV on zoom */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Zoom|Getters")
	FORCEINLINE bool GetIfZoomAdjustsFOV() const { return bZoomFOV; }
	/* returns if the panning adjusts rotation  on zoom */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Panning|Getters")
	FORCEINLINE bool GetIfRotationAdjustsRotation() const { return bRotateOnPan; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Movement|Getters")
	FORCEINLINE FMovementSettings GetCameraMovementSpeedSettings() const { return MovementSpeedSettings; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Movement|Getters")
	FORCEINLINE float GetDefaultMovementSpeed() const { return DefaultMovementSpeed; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Pan|Getters")
	FORCEINLINE FMovementSettings GetPanSpeedSettings() const { return PanSpeedSettings; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Pan|Getters")
	FORCEINLINE float GetPanSpeed() const { return PanSpeed; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Pan|Getters")
	FORCEINLINE float GetDefaultPanSpeed() const { return DefaultPanSpeed; }

public:
	/* Interface wrapper that sets spring arm length during zoom events. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallSetArmlength(float Value);
	void CallSetArmlength_Implementation(float Value);
	/* Interface wrapper that sets the pawn rotation during zoom events. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallSetArmRotation(float Value);
	void CallSetArmRotation_Implementation(float Value);
	/* Interface wrapper that sets the camera FOV during zoom events. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallSetFieldOfView(float Value);
	void CallSetFieldOfView_Implementation(float Value);
	/* Interface event that pans the camera (side to side). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallPanCamera(float Value);
	void CallPanCamera_Implementation(float Value);
	/* Interface event that pans the camera (up and down). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallRotateCamera(float Value);
	void CallRotateCamera_Implementation(float Value);
	/* Interface event resets spring arm rotation */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallResetRotation();
	void CallResetRotation_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallSetDoAdvanceCamerCollision(bool NewValue);
	void CallSetDoAdvanceCamerCollision_Implementation(bool NewValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Movement")
	void CallGetClampedSpringArmLength(float& NewLengthOut);
	void CallGetClampedSpringArmLength_Implementation(float& NewLengthOut);

	/* Helper function that checks if the camera 1) does NOT rotate on zoom and 2) is allowed to roate on pan */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Getters")
	FORCEINLINE bool GetShouldRotateOnPan() const { return !bZoomRotation && bRotateOnPan; }



protected:
	/* Runs check to determine if selected target can be tracked. If so it begins tracking. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Tracking")
	void TrackObject();
	void TrackObject_Implementation();
	/* Sets target actor to follow, and begins timer to move camera from current location to target. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Tracking")
	void AttachAsFollowCamera(AActor* FollowActor, bool Bypass = false);
	void AttachAsFollowCamera_Implementation(AActor* FollowActor, bool Bypass = false);
	/* Lerps the camera from current location to target. Invalidates itself when it reaches the target. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Tracking")
	void AttachTimer();
	void AttachTimer_Implementation();
	/* Sets variables used to say that the pawn is tracking, opening the gate to tracking events */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Tracking")
	void OpenFollow();
	void OpenFollow_Implementation();
	/* Sets variables used to say that the pawn is NOT tracking, closing the gate to tracking events */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Tracking")
	void CloseFollow();
	void CloseFollow_Implementation();
	/* Tracking events run on tick. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Tracking")
	void RunFollow();
	void RunFollow_Implementation();
	/* To break tracking through another means other than clicking again - useful on WADS movement */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Tracking")
	void ForceBreakFollow();
	void ForceBreakFollow_Implementation();

	/* Sets if the pawn can track objects in the world. Disables current tracking (can skip)*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|Tracking")
	void SetCanTrack(bool CanTrack, bool DisableCurrentTracking = true);
	void SetCanTrack_Implementation(bool CanTrack, bool DisableCurrentTracking = true);

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|MovementSettings")
	void PushMovementSpeed(float NewSpeed);
	void PushMovementSpeed_Implementation(float NewSpeed);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera|MovementSettings")
	void PushPanSensitivity(float NewPanSensitivity);
	void PushPanSensitivity_Implementation(float NewPanSensitivity);

public:
	/* Returns if the pawn can track objects. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Tracking|Getters")
	FORCEINLINE bool GetCanTrack() const { return bCanTrack; }
	/* Returns the track objects. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Tracking|Getters")
	FORCEINLINE AActor* GetFollowTargetRef() const { return FollowTargetRef; }
	/* returns if the Camera is attaching to a track target */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Tracking|Getters")
	FORCEINLINE bool GetIsAttaching() const { return bIsAttaching; }
	/* returns if camera is tracking a target */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera|Tracking|Getters")
	FORCEINLINE bool GetIsTracking() const { return bIsTracking; }

	/* returns the sphere root */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "General|Getters")
	FORCEINLINE USphereComponent* GetCollisionSphere() const { return CollisionSphere;  }
	/* returns the spring arm */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "General|Getters")
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return CameraSpringArm; }
	/* returns the player camera */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "General|Getters")
	FORCEINLINE UCameraComponent* GetPlayerCamera() const { return PlayerCamera; }
};


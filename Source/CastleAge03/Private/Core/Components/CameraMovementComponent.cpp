// Custom Movement Component (for custom camera pawn), written by Zachary J. Parker (2021), adapted from earlier work.
// Custom Movement Component code may be used for this project and has been previously used by the above in a tutorial.

#include "Core/Components/CameraMovementComponent.h"
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Core/Player/CameraPawn.h"
#include "Core/Components/CameraMovementComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UCameraMovementComponent::UCameraMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;

	// Set default attributes
	bDisableCameraMovement = false;
	bDisableEdgeScroll = false;
	DefaultMovementSpeed = 15.0f;
	MovementSpeedModifier = 1.0f;
	CornerEdgeScrollSpeed = 0.707106781 * DefaultMovementSpeed; // 0.707106781 = rounded sqrt(.5). Using this value as it is less costly than running the sqrt().
	DeltaZoomAmount = 0.5f;
	ZoomAmount = 0.5f;
	
}

// Called when the game starts
void UCameraMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	//CameraOwner = Cast<ACameraPawn>(GetOwner()); // populates owner of this component - yes it is a cast. No there is no issue with it being a cast.
	PlayerControllerRef = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)); // populates Player Controller - yes it is a cast. No there is no issue with it being a cast.
	//PlayerControllerRef->bShouldPerformFullTickWhenPaused(true)
	//GetOwner()->GetClass()->ImplementsInterface(UCameraPawnInterface::StaticClass());
}

// Called every frame
void UCameraMovementComponent::TickComponent(float InDeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(InDeltaTime, TickType, ThisTickFunction);

	// Camera lag (consider removing)
	ZoomAmount = FMath::Lerp(ZoomAmount, DeltaZoomAmount, 0.1f);

	// Update Camera Zoom per Tick
	ICameraPawnInterface::Execute_CallSetArmlength(GetOwner(), ZoomAmount);
	ICameraPawnInterface::Execute_CallSetArmRotation(GetOwner(), ZoomAmount);
	ICameraPawnInterface::Execute_CallSetFieldOfView(GetOwner(), ZoomAmount);
}

// METHODS DEFINITIONS (Getters, Setters, Movement controls)
// 
// GETTERS
// TODO: MAKE THESE INLINE YOU GOD DAMN DUMBASS!
FVector UCameraMovementComponent::GetCameraLocation()
{
	return GetOwner()->GetActorLocation();
}

FVector UCameraMovementComponent::GetFaceDirection()
{
	return GetOwner()->GetActorForwardVector();
}

FRotator UCameraMovementComponent::GetCameraRotation()
{
	return GetOwner()->GetActorRotation();
}

// SETTERS

/** Set Speed Modifier (player controller passes in value of 1 or 2)*/
float UCameraMovementComponent::SetMovementSpeedModifier(float ModifierAmount)
{
	return MovementSpeedModifier = ModifierAmount;
}

/** Set Movement Speed*/
float UCameraMovementComponent::SetMovementSpeed(float MovementSpeedAdjustment)
{
	return DefaultMovementSpeed = MovementSpeedAdjustment;
}

/** Disable/Enable Camera Movement */
bool UCameraMovementComponent::SetCameraDiabled(bool bDisableCamera)
{
	return bDisableCameraMovement = bDisableCamera;
}

/** Disable/Enable Edge Scroll*/
bool UCameraMovementComponent::SetDisableEdgeScroll(bool bDisableScroll)
{
	return bDisableEdgeScroll = bDisableScroll;
}

// MOVEMENT CONTROLS

/** Movement controls W A S D*/
void UCameraMovementComponent::BasicMovementControl(float AxisValueX, float AxisValueY)
{
	// Set vaiables used in calculation, we do not want to eat procesor time by running this constnatly.
	// All local scope
	float MovementValueX;
	float MovementValueY;
	FVector MovementX;
	FVector MovementY;

	if (IsCameraDisabled() == false)
	{
		if (AxisValueX != 0 && AxisValueY != 0) // Both a up/down AND a left/right key is being triggered
		{
			MovementValueX = AxisValueX * GetCurrentMovementSpeed() * GetSpeedModifier(); // calculates what the x movement value should be
			MovementX = FVector(MovementValueX, 0, 0); // sets the x movement value

			MovementValueY = AxisValueY * GetCurrentMovementSpeed() * GetSpeedModifier(); /// calculates what the y movement value should be
			MovementY = FVector(0, MovementValueY, 0); // Sets the y movement value

			GetOwner()->AddActorLocalOffset(MovementX + MovementValueY, true); // move!
		}
		else if (AxisValueX != 0) // ONLY up/down has been triggered (this is y in screen space and x in world space)
		{
			MovementValueX = AxisValueX * GetCurrentMovementSpeed() * GetSpeedModifier(); // calculates what the x movement value should be
			MovementX = FVector(MovementValueX, 0, 0); // sets the x movement value

			GetOwner()->AddActorLocalOffset(MovementX, true); // move!
		}
		else if (AxisValueY != 0) // ONLY left/right has been trigerred (this is x in screen space and y in world space)
		{
			MovementValueY = AxisValueY * GetCurrentMovementSpeed() * GetSpeedModifier(); /// calculates what the y movement value should be
			MovementY = FVector(0, MovementValueY, 0); // Sets the y movement value

			GetOwner()->AddActorLocalOffset(MovementY, true); // move!
		}
	}
}

/** Camera Pan */
void UCameraMovementComponent::PanCamera(float RotationAmount)
{
	FRotator NewRotationLocal = GetCameraRotation().Add(0, RotationAmount, 0);
	GetOwner()->SetActorRotation(NewRotationLocal);
}

/** Reset Camera Pawn */
void UCameraMovementComponent::ResetPan()
{
	GetOwner()->SetActorRotation(FRotator(0.0, 0.0, 0.0)); // double check this is accurate.
}

/** Edge Scroll - X and Y are reversed on screen from game world. */
void UCameraMovementComponent::EdgeScroll()
{
	// variables for momvent all local scope
	float DeltaSpeedX;
	float DeltaSpeedY;
	FVector MovementX; // Use this with DeltaY
	FVector MovementY; // use this with DeltaX

	// Get Mouse position
	float MousePositionX{ 0 };
	float MousePositionY{ 0 };
	PlayerControllerRef->GetMousePosition(MousePositionX, MousePositionY);

	// Get Viewport (gameplay window area) size
	int32 ViewportSizeX;
	int32 ViewportSizeY;
	PlayerControllerRef->GetViewportSize(ViewportSizeX, ViewportSizeY);

	// Calculate Proportions (we want these calculations to be updated with mouse movement)
	float ProportionX = MousePositionX / ViewportSizeX;
	float ProportionY = MousePositionY / ViewportSizeY;

	// Set Edge Scroll Speeds based on Proportions. 
	if (IsEdgeScrollDisabled() == false)
	{
		if (ProportionX >= .975 && ProportionY <= .025) // TOP RIGHT corner
		{
			DeltaSpeedX = CornerEdgeScrollSpeed * GetSpeedModifier();
			DeltaSpeedY = CornerEdgeScrollSpeed;

			MovementX = FVector(DeltaSpeedY, 0, 0); // Switch due to screen vs world space
			MovementY = FVector(0, DeltaSpeedX, 0); // Switch due to screen versus world space 

			GetOwner()->AddActorLocalOffset(MovementX + MovementY, true); // Edge scroll!
			SetCameraDiabled(true); // prevents W A D S and edge scroll.
		}
		else if (ProportionX >= .975 && ProportionY >= .975) // BOTTOM RIGHT corner
		{
			DeltaSpeedX = CornerEdgeScrollSpeed * GetSpeedModifier();
			DeltaSpeedY = CornerEdgeScrollSpeed * -1.0f;

			MovementX = FVector(DeltaSpeedY, 0, 0); // Switch due to screen vs world space
			MovementY = FVector(0, DeltaSpeedX, 0); // Switch due to screen versus world space 

			GetOwner()->AddActorLocalOffset(MovementX + MovementY, true); // Edge scroll!
			SetCameraDiabled(true); // prevents W A D S and edge scroll.
		}
		else if (ProportionX <= .025 && ProportionY <= .025) // TOP LEFT corner
		{
			DeltaSpeedX = CornerEdgeScrollSpeed * -1.0f * GetSpeedModifier();
			DeltaSpeedY = CornerEdgeScrollSpeed;

			MovementX = FVector(DeltaSpeedY, 0, 0); // Switch due to screen vs world space
			MovementY = FVector(0, DeltaSpeedX, 0); // Switch due to screen versus world space 

			GetOwner()->AddActorLocalOffset(MovementX + MovementY, true); // Edge scroll!
			SetCameraDiabled(true); // prevents W A D S and edge scroll.
		}
		else if (ProportionX <= .025 && ProportionY >= .975) // BOTTOM LEFT corner
		{
			DeltaSpeedX = CornerEdgeScrollSpeed * -1.0f;  GetSpeedModifier();
			DeltaSpeedY = CornerEdgeScrollSpeed * -1.0f;

			MovementX = FVector(DeltaSpeedY, 0, 0); // Switch due to screen vs world space
			MovementY = FVector(0, DeltaSpeedX, 0); // Switch due to screen versus world space 

			GetOwner()->AddActorLocalOffset(MovementX + MovementY, true); // Edge scroll!
			SetCameraDiabled(true); // prevents W A D S and edge scroll.
		}
		else if (ProportionX >= .975) // RIGHT
		{
			DeltaSpeedX = DefaultMovementSpeed * GetSpeedModifier();
			DeltaSpeedY = 0.0f;

			MovementY = FVector(0, DeltaSpeedX, 0); // Switch due to screen versus world space 

			GetOwner()->AddActorLocalOffset(MovementY, true); // Edge scroll!
			SetCameraDiabled(true); // prevents W A D S and edge scroll.
		}
		else if (ProportionX <= .025) // LEFT
		{
			DeltaSpeedX = DefaultMovementSpeed * -1.0f * GetSpeedModifier();
			DeltaSpeedY = 0.0f;

			MovementY = FVector(0, DeltaSpeedX, 0); // Switch due to screen versus world space 

			GetOwner()->AddActorLocalOffset(MovementY, true); // Edge scroll!
			SetCameraDiabled(true); // prevents W A D S and edge scroll.
		}
		else if (ProportionY >= .975) // BOTTOM
		{
			DeltaSpeedX = 0.0f;
			DeltaSpeedY = DefaultMovementSpeed * -1.0f * GetSpeedModifier();


			MovementX = FVector(DeltaSpeedY, 0, 0); // Switch due to screen versus world space 

			GetOwner()->AddActorLocalOffset(MovementX, true); // Edge scroll!
			SetCameraDiabled(true); // prevents W A D S and edge scroll.
		}
		else if (ProportionY <= .025) // TOP
		{
			DeltaSpeedX = 0.0f;
			DeltaSpeedY = DefaultMovementSpeed * GetSpeedModifier();

			MovementX = FVector(DeltaSpeedY, 0, 0); // Switch due to screen versus world space 

			GetOwner()->AddActorLocalOffset(MovementX, true); // Edge scroll!
			SetCameraDiabled(true); // prevents W A D S and edge scroll.
		}
		else // catchall
		{
			DeltaSpeedX = 0.0f;
			DeltaSpeedY = 0.0f;
			SetCameraDiabled(false); // ensures camera works when not edge scrolling
		}
	}
}

/** Zoom In*/
void UCameraMovementComponent::ZoomIn()
{
	DeltaZoomAmount += 0.1f;
	if (DeltaZoomAmount > 1)
	{
		DeltaZoomAmount = 1; // locks the zoom in max to this value
	}
#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("%F"), ZoomAmount);
#endif
}

/** Zoom Out*/
void UCameraMovementComponent::ZoomOut()
{
	DeltaZoomAmount -= 0.1f;
	if (DeltaZoomAmount < 0)
	{
		DeltaZoomAmount = 0; // locks the zoom out max to this value
	}
#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("%F"), ZoomAmount);
#endif
}

/** Zoom Reset*/
void UCameraMovementComponent::ResetZoom()
{
	DeltaZoomAmount = 0.5f;
}

/** Camera Reset*/
void UCameraMovementComponent::CameraReset()
{
	ResetZoom();
	ResetPan();
}
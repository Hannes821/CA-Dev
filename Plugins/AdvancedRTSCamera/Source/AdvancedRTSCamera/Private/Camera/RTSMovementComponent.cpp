// Copyright Two Neurons Studio, LLC. Year of Publication 2024.


#include "Camera/RTSMovementComponent.h"
#include "Interfaces/RTSMovementInterface.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

URTSMovementComponent::URTSMovementComponent()
{
	// By default the camera can move when the game is paused.
	// Tick is needed for movement based calculations. 
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
}


// Called when the game starts
void URTSMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerControllerRef = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)); // populates Player Controller - yes it is a cast. No there is no issue with it being a cast.
	
	// Set the movement speed modifier 
	UpdateMovementSpeedModifier();

	// Set the edge movement speeds for normal (WADS) and edge scroll movement.
	CalculateCornerMovementSpeed();
	CalculateCornerEdgeScrollSpeed();
	SetEdgeScrollTriggers();

	// Ensure the Zoom and ZoomAmount are the same at the start
	SetZoomAmount();


	if (IsEdgeScrollAlwaysDisabled())
	{
		SetDisableEdgeScroll(true);
	}

	if (GetOwner()->GetClass()->ImplementsInterface(URTSMovementInterface::StaticClass()))
	{
		bMovementInterfaceConnected = true;
	}
	else
	{
		bMovementInterfaceConnected = false;
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Owner does NOT implement Movement Interface!!!"));
#endif
	}
}


// Called every frame
void URTSMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	EdgeScroll();
	ZoomEvents();
}

void URTSMovementComponent::SetDefaultMovementSpeed_Implementation(float NewSpeed)
{
	DefaultMovementSpeed = NewSpeed;
	CalculateCornerMovementSpeed();
	CalculateCornerEdgeScrollSpeed();
}

void URTSMovementComponent::UpdateMovementSpeedModifier_Implementation()
{
	MovementSpeedModifier = DefaultMovementSpeedModifier;
}

void URTSMovementComponent::SetMovementSpeedModifier_Implementation(float NewModifier)
{
	DefaultMovementSpeedModifier = NewModifier;
	UpdateMovementSpeedModifier();
}

void URTSMovementComponent::CalculateCornerMovementSpeed_Implementation()
{
	if (!bOverrideCornerMovementSpeed)
	{
		CornerEdgeScrollSpeed = DefaultMovementSpeed * sqrt(.5);
	}
}

void URTSMovementComponent::CalculateCornerEdgeScrollSpeed_Implementation()
{
	if (!bOverrideEdgeScrollSpeed)
	{
		CornerEdgeScrollSpeed = DefaultMovementSpeed * sqrt(.5);
	}
}

void URTSMovementComponent::SetEdgeScrollTriggers_Implementation()
{
	EdgeScrollLowValue = EdgeScrollTriggerValue;
	EdgeScrollHighValue = 1 - EdgeScrollTriggerValue;
}

void URTSMovementComponent::SetZoomAmount_Implementation()
{
	Zoom = ZoomAmount;
	DefaultZoomAmount = ZoomAmount;
}

void URTSMovementComponent::TriggerMovementSpeedModifier_Implementation()
{
	if (bMovementModifierActive)
	{
		MovementSpeedModifier = DefaultMovementSpeedModifier;
		bMovementModifierActive = false;
	}
	else
	{
		MovementSpeedModifier = ActiveMovementSpeedModifier;
		bMovementModifierActive = true;
	}
}

void URTSMovementComponent::ToggleMovementSpeedModifier_Implementation(bool ModifyMovement)
{
	if (ModifyMovement)
	{
		MovementSpeedModifier = ActiveMovementSpeedModifier;
		bMovementModifierActive = true;
	}
	else
	{
		MovementSpeedModifier = DefaultMovementSpeedModifier;
		bMovementModifierActive = false;
	}
}

void URTSMovementComponent::SetCameraDisabled_Implementation(bool DisableCamera)
{
	bDisableCameraMovement = DisableCamera;
}

void URTSMovementComponent::SetCameraDisabledDueToEdgeScroll_Implementation(bool DisableCamera)
{
	bDisableMovementDueToEdgeScroll = DisableCamera;
}

void URTSMovementComponent::SetDisableEdgeScroll_Implementation(bool DisableEdgeScroll)
{
	if (!IsEdgeScrollAlwaysDisabled())
	{
		bDynamicallyDisableEdgeScroll = DisableEdgeScroll;
	}
	else
	{
		bDynamicallyDisableEdgeScroll = true;
	}
}

void URTSMovementComponent::SetDisablePanning_Implementation(bool DisablePanning)
{
	bDisablePanning = DisablePanning;
}

void URTSMovementComponent::BasicMovementControl_Implementation(float AxisValueX, float AxisValueY)
{
	// Local variables used to set movement.
	float MovementValueXLocal;
	float MovementValueYLocal;
	FVector MovementXLocal;
	FVector MovementYLocal;

	if (!IsCameraDisabled() && !IsDisableDueToEdgeScroll())
	{
		if (AxisValueX != 0.f && AxisValueY != 0) // movement on both X and Y plane
		{
			MovementValueXLocal = AxisValueX * GetCornerMovementSpeed() * GetSpeedModifier();
			MovementValueYLocal = AxisValueY * GetCornerMovementSpeed() * GetSpeedModifier();

			MovementXLocal = FVector(MovementValueXLocal, 0, 0);
			MovementYLocal = FVector(0, MovementValueYLocal, 0);

			MoveForwardRight(MovementXLocal, MovementYLocal);


		}
		else if (AxisValueX != 0.f && AxisValueY == 0) // Only moving on the relative forward plane (this is Y in screenspace and X in worldspace)
		{
			MovementValueXLocal = AxisValueX * GetCurrentMovementSpeed() * GetSpeedModifier();
			MovementXLocal = FVector(MovementValueXLocal, 0, 0);

			MoveForward(MovementXLocal);

		}
		else if (AxisValueY != 0.f && AxisValueX == 0) // Only moving on the relative right plane (this is X in screenspace and Y in worldspace)
		{
			MovementValueYLocal = AxisValueY * GetCurrentMovementSpeed() * GetSpeedModifier();
			MovementYLocal = FVector(0, MovementValueYLocal, 0);

			MoveRight(MovementYLocal);

		}
	}
}

void URTSMovementComponent::MoveForward_Implementation(FVector MovementX)
{
	GetOwner()->AddActorLocalOffset(MovementX, true);
}

void URTSMovementComponent::MoveRight_Implementation(FVector MovementY)
{
	GetOwner()->AddActorLocalOffset(MovementY, true);
}

void URTSMovementComponent::MoveForwardRight_Implementation(FVector MovementX, FVector MovementY)
{
	GetOwner()->AddActorLocalOffset(MovementX + MovementY, true);
}

void URTSMovementComponent::EdgeScroll_Implementation()
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
	

	if (!IsEdgeScrollDisabled())
	{
		if (ProportionX >= EdgeScrollHighValue && ProportionY <= EdgeScrollLowValue) // TOP RIGHT corner
		{
			DeltaSpeedX = CornerEdgeScrollSpeed * GetSpeedModifier();
			DeltaSpeedY = CornerEdgeScrollSpeed;

			MovementX = FVector(DeltaSpeedY, 0, 0); // Switch due to screen vs world space
			MovementY = FVector(0, DeltaSpeedX, 0); // Switch due to screen versus world space 

			GetOwner()->AddActorLocalOffset(MovementX + MovementY, true); 
			SetCameraDisabledDueToEdgeScroll(true); // prevents W A D S and edge scroll.
		}
		else if (ProportionX >= EdgeScrollHighValue && ProportionY >= EdgeScrollHighValue) // BOTTOM RIGHT corner
		{
			DeltaSpeedX = CornerEdgeScrollSpeed * GetSpeedModifier();
			DeltaSpeedY = CornerEdgeScrollSpeed * -1.0f;

			MovementX = FVector(DeltaSpeedY, 0, 0); 
			MovementY = FVector(0, DeltaSpeedX, 0); 

			GetOwner()->AddActorLocalOffset(MovementX + MovementY, true);
			SetCameraDisabledDueToEdgeScroll(true);
		}
		else if (ProportionX <= EdgeScrollLowValue && ProportionY <= EdgeScrollLowValue) // TOP LEFT corner
		{
			DeltaSpeedX = CornerEdgeScrollSpeed * -1.0f * GetSpeedModifier();
			DeltaSpeedY = CornerEdgeScrollSpeed;

			MovementX = FVector(DeltaSpeedY, 0, 0); 
			MovementY = FVector(0, DeltaSpeedX, 0); 

			GetOwner()->AddActorLocalOffset(MovementX + MovementY, true); // Edge scroll!
			SetCameraDisabledDueToEdgeScroll(true);
		}
		else if (ProportionX <= EdgeScrollLowValue && ProportionY >= EdgeScrollHighValue) // BOTTOM LEFT corner
		{
			DeltaSpeedX = CornerEdgeScrollSpeed * -1.0f;  GetSpeedModifier();
			DeltaSpeedY = CornerEdgeScrollSpeed * -1.0f;

			MovementX = FVector(DeltaSpeedY, 0, 0); 
			MovementY = FVector(0, DeltaSpeedX, 0);  

			GetOwner()->AddActorLocalOffset(MovementX + MovementY, true); 
			SetCameraDisabledDueToEdgeScroll(true);
		}
		else if (ProportionX >= EdgeScrollHighValue) // RIGHT
		{
			DeltaSpeedX = DefaultMovementSpeed * GetSpeedModifier();
			DeltaSpeedY = 0.0f;

			MovementY = FVector(0, DeltaSpeedX, 0); 

			GetOwner()->AddActorLocalOffset(MovementY, true); 
			SetCameraDisabledDueToEdgeScroll(true);
		}
		else if (ProportionX <= EdgeScrollLowValue) // LEFT
		{
			DeltaSpeedX = DefaultMovementSpeed * -1.0f * GetSpeedModifier();
			DeltaSpeedY = 0.0f;

			MovementY = FVector(0, DeltaSpeedX, 0); 

			GetOwner()->AddActorLocalOffset(MovementY, true);
			SetCameraDisabledDueToEdgeScroll(true);
		}
		else if (ProportionY >= EdgeScrollHighValue) // BOTTOM
		{
			DeltaSpeedX = 0.0f;
			DeltaSpeedY = DefaultMovementSpeed * -1.0f * GetSpeedModifier();


			MovementX = FVector(DeltaSpeedY, 0, 0);

			GetOwner()->AddActorLocalOffset(MovementX, true); 
			SetCameraDisabledDueToEdgeScroll(true);
		}
		else if (ProportionY <= EdgeScrollLowValue) // TOP
		{
			DeltaSpeedX = 0.0f;
			DeltaSpeedY = DefaultMovementSpeed * GetSpeedModifier();

			MovementX = FVector(DeltaSpeedY, 0, 0); 

			GetOwner()->AddActorLocalOffset(MovementX, true); 
			SetCameraDisabledDueToEdgeScroll(true);
		}
		else // catchall
		{
			DeltaSpeedX = 0.0f;
			DeltaSpeedY = 0.0f;
			SetCameraDisabledDueToEdgeScroll(false); // ensures camera works when not edge scrolling
		}
	}
}

void URTSMovementComponent::PanCamera_Implementation(float DeltaRotation)
{
	if (!IsPanningDisabled())
	{
		if (bDisableEdgeScrollWhilePanning)
		{
			bDynamicallyDisableEdgeScroll = true;
		}
		IRTSMovementInterface::Execute_CallPanCamera(GetOwner(), DeltaRotation);
	}
}

void URTSMovementComponent::RotateCamera_Implementation(float DeltaRotation)
{
	if (!IsPanningDisabled())
	{
		if (bDisableEdgeScrollWhilePanning)
		{
			bDynamicallyDisableEdgeScroll = true;
		}
		IRTSMovementInterface::Execute_CallRotateCamera(GetOwner(), DeltaRotation);
	}
}

void URTSMovementComponent::ResetPan_Implementation()
{
	GetOwner()->SetActorRotation(DefaultRotation);
	IRTSMovementInterface::Execute_CallResetRotation(GetOwner());
}

void URTSMovementComponent::ZoomEvents_Implementation()
{
	if (bMovementInterfaceConnected)
	{
		IRTSMovementInterface::Execute_CallSetArmlength(GetOwner(), ZoomAmount);
		IRTSMovementInterface::Execute_CallSetArmRotation(GetOwner(), ZoomAmount);
		IRTSMovementInterface::Execute_CallSetFieldOfView(GetOwner(), ZoomAmount);
	}
}

void URTSMovementComponent::ZoomIn_Implementation()
{
	if (bSingleClickZoom)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(SingleClickZoomOutTimerHandle)) // clear old timer
		{
			GetWorld()->GetTimerManager().ClearTimer(SingleClickZoomOutTimerHandle);
			SingleClickZoomOutTimerHandle.Invalidate();
		}

		if (!GetWorld()->GetTimerManager().IsTimerActive(SingleClickZoomInTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(SingleClickZoomInTimerHandle, this, &URTSMovementComponent::SingleClickZoomIn, SingleClickZoomSpeed, true);
		}
	}
	else
	{
		ZoomAmount += DeltaZoom;
		if (ZoomAmount > 1)
		{
			ZoomAmount = 1;
		}
	}
}

void URTSMovementComponent::ZoomOut_Implementation()
{
	if (bSingleClickZoom)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(SingleClickZoomInTimerHandle)) // clear old timer
		{
			GetWorld()->GetTimerManager().ClearTimer(SingleClickZoomInTimerHandle);
			SingleClickZoomInTimerHandle.Invalidate();
		}

		if (!GetWorld()->GetTimerManager().IsTimerActive(SingleClickZoomOutTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(SingleClickZoomOutTimerHandle, this, &URTSMovementComponent::SingleClickZoomOut, SingleClickZoomSpeed, true);
		}
	}
	else
	{
		ZoomAmount -= DeltaZoom;
		if (ZoomAmount < 0)
		{
			ZoomAmount = 0;
		}
	}
}

void URTSMovementComponent::ResetZoom_Implementation()
{
	ZoomAmount = DefaultZoomAmount;
}

void URTSMovementComponent::ResetCamera_Implementation()
{
	ResetZoom();
	ResetPan();
}

void URTSMovementComponent::SingleClickZoomIn_Implementation()
{
	ZoomAmount += DeltaZoom;
	if (ZoomAmount > 1)
	{
		ZoomAmount = 1;
		GetWorld()->GetTimerManager().ClearTimer(SingleClickZoomInTimerHandle);
		SingleClickZoomInTimerHandle.Invalidate();
	}
}

void URTSMovementComponent::SingleClickZoomOut_Implementation()
{
	ZoomAmount -= DeltaZoom;
	if (ZoomAmount < 0)
	{
		ZoomAmount = 0;
		GetWorld()->GetTimerManager().ClearTimer(SingleClickZoomOutTimerHandle);
		SingleClickZoomOutTimerHandle.Invalidate();
	}
}
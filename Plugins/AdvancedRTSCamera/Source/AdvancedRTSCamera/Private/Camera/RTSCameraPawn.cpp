// Copyright Two Neurons Studio, LLC. Year of Publication 2024.


#include "Camera/RTSCameraPawn.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/RTSCameraFadeInterface.h"

// Sets default values
ARTSCameraPawn::ARTSCameraPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	bReplicates = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(CollisionSphere);
	CollisionSphere->InitSphereRadius(32.0);
	CollisionSphere->SetWorldScale3D(FVector(0.25, 0.25, 0.25));


	// Create Camera Boom (spring arm) and attach it to root (collission sphere)
	CameraSpringArm = CreateDefaultSubobject<URTSSpringArm>(TEXT("CameraBoom"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->SetUsingAbsoluteRotation(false);
	CameraSpringArm->TargetArmLength = DefaultZoomDistance; // this is also the default zoom length
	CameraSpringArm->SetRelativeRotation(DefaultCameraRotation); // Pitch (Y), Yaw (Z), Roll (X)
	CameraSpringArm->bDoCollisionTest = false;
	CameraSpringArm->bInheritRoll = false;
	CameraSpringArm->bEnableCameraLag = true; // makes for smoother movement
	CameraSpringArm->CameraLagSpeed = 10.0f;
	CameraSpringArm->bEnableCameraRotationLag = true; // lag is usful for preventing 'jerking' motion
	CameraSpringArm->CameraRotationLagSpeed = 10.0f;
	CameraSpringArm->bInheritPitch = false; // needed to ensure zoom is updated in real time.


	//create Camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraSpringArm, URTSSpringArm::SocketName);
	PlayerCamera->FieldOfView = DefaultFieldOfView;

	//Attach Movement Component to Camera Pawn
	PawnMovementComponent = CreateDefaultSubobject<URTSMovementComponent>(TEXT("PawnMovementComponent"));

	// Default settings for inheriting controller rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraHeightCollisonParams.AddIgnoredActor(this);
	CameraHeightCollisonParams.AddIgnoredActors(ActorsToIgnore);
	CameraHeightCollisonParams.bTraceComplex = bHeightAdjustTraceComplex;

	CameraFadeCollisonParams.AddIgnoredActor(this);
	CameraHeightCollisonParams.bTraceComplex = true;

	AdjustDesiredHeightToTarget();
	DefaultMovementSpeed = MovementSpeedSettings.Current;
	DefaultPanSpeed = PanSpeedSettings.Current;
}

// Called when the game starts or when spawned
void ARTSCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)); // populates Player Controller - yes it is a cast. No there is no issue with it being a cast.

	// preload settings events
	SetMovementSpeed();
	SetPanSpeed();
	

	DefaultCameraRotation = CameraSpringArm->GetRelativeRotation();
	CameraSpringArm->SetDoesAdvanceCollisionTest(bDoAdvanceCollisionTest);
	if (bDoAdvanceCollisionTest)
	{
		CameraSpringArm->SetSpringArmLengthMin(SpringArmZoomIn);
		CameraSpringArm->SetSpringArmLengthMax(SpringArmZoomOut);
	}

	if (!bAdjustPawnHeightForTerrain)
	{
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetDesiredCameraHeight()), false);
	}
	else
	{
		SetTargetHeight(GetDesiredCameraHeight());
	}

	if (bStartAttachedToTrack && StartingFollowTarget)
	{
		AttachAsFollowCamera(StartingFollowTarget);
	}
}

// Called every frame
void ARTSCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickEvents(DeltaTime);
}


// Called to bind functionality to input
void ARTSCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARTSCameraPawn::TickEvents_Implementation(float DeltaTime)
{

	if (bDoAdvanceCollisionTest)
	{
		CameraSpringArm->AdvanedCollisionTest(ClampedSpringArmMax);
		AdjustCameraSpringArmLength();
	}

	if (!bIsTracking && !bIsAttaching)
	{
		if (bAdjustPawnHeightForTerrain)
		{
			AdjustCameraHeight(DeltaTime);
		}
	}

	else if (bIsTracking && !bIsAttaching)
	{
		RunFollow();
	}

	if (bRunFadeCheck)
	{
		FadeEvent();
	}
}

void ARTSCameraPawn::AdjustCameraHeight_Implementation(float DeltaTime)
{
	// determine start and end points for line trace
	FVector StartLocationLocal = GetActorLocation();
	FVector EndLocationLocal = GetActorLocation() - FVector(0.0f, 0.0f, 100000.0f);

	// declare results
	FHitResult OutHitLocal;

	// Start Trace
	GetWorld()->LineTraceSingleByChannel(OutHitLocal, StartLocationLocal, EndLocationLocal, CameraHeightChannel, CameraHeightCollisonParams);

	// Check if at correct height - runs if pawn needs to be adjusted
	float DistanceLocal = FVector::Dist(OutHitLocal.ImpactPoint, GetActorLocation());

	if (!FMath::IsNearlyEqual(DistanceLocal, TargetCameraHeight, CameraHeightTolerance))
	{
		AdjustCameraAlpha += DeltaTime;
		if (AdjustCameraAlpha >= 1)
			AdjustCameraAlpha = 1;

		if (DistanceLocal > TargetCameraHeight)
		{
			FVector NewLocationLocal = GetActorLocation() - FVector(0.0f, 0.0f, DistanceLocal - TargetCameraHeight);
			SetActorLocation(FMath::InterpEaseInOut(GetActorLocation(), NewLocationLocal, AdjustCameraAlpha, CameraHeightAdjustExp), true);

		}
		else if (DistanceLocal < TargetCameraHeight)
		{
			FVector NewLocationLocal = GetActorLocation() + FVector(0.0f, 0.0f, DistanceLocal + (TargetCameraHeight + 101.f));
			SetActorLocation(FMath::InterpEaseInOut(GetActorLocation(), NewLocationLocal, AdjustCameraAlpha, CameraHeightAdjustExp), true);
		}
	}
	else
	{
		AdjustCameraAlpha = 0.f;
	}
}

void ARTSCameraPawn::AdjustCameraSpringArmLength_Implementation()
{
	CameraSpringArm->TargetArmLength = ClampedSpringArmMax;
}

void ARTSCameraPawn::FadeEvent_Implementation()
{
	// determine start and end points for line trace
	FVector StartLocationLocal = PlayerCamera->GetComponentLocation();
	FVector EndLocationLocal = (PlayerCamera->GetComponentLocation() + (PlayerCamera->GetForwardVector() * FadeTraceDistance));

	// declare results
	FHitResult OutHitLocal;

	if (GetWorld()->LineTraceSingleByChannel(OutHitLocal, StartLocationLocal, EndLocationLocal, CameraFadeChannel, CameraFadeCollisonParams))
	{
		if (OutHitLocal.GetActor()->GetClass()->ImplementsInterface(URTSCameraFadeInterface::StaticClass()))
		{
			SetFadeTarget(OutHitLocal.GetActor());
			RunFadeTarget();
		}
	}
	else
	{
		if (FadeTarget)
		{

			RestoreTargetMaterials();
			SetFadeTarget(nullptr);

		}
	}

}

void ARTSCameraPawn::SetFadeTarget_Implementation(AActor* Target /* = nullptr*/)
{
	FadeTarget = Target;
}

void ARTSCameraPawn::RunFadeTarget_Implementation()
{
	if (FadeTarget)
	{
		if (!bHasFadeTarget) // DoOnce
		{
			bHasFadeTarget = true; // Close DoOnce

			// store mesh
			FadeMesh = IRTSCameraFadeInterface::Execute_GetStaticMesh(FadeTarget);

			// store materials
			for (size_t i = 0; i < FadeMesh->GetMaterials().Num(); i++)
			{
				OriginalMaterials.Add(FadeMesh->GetMaterials()[i]);
			}

			// OriginalMaterials = FadeMesh->GetMaterials();

			// place fade material 
			for (size_t i = 0; i < OriginalMaterials.Num(); i++)
			{
				IRTSCameraFadeInterface::Execute_SetMaterial(FadeTarget, FadeMesh, i, FadeMaterial);
			}

		}
	}
}

void ARTSCameraPawn::RestoreTargetMaterials_Implementation()
{
	bHasFadeTarget = false; // Reset DoOnce

	for (size_t i = 0; i < OriginalMaterials.Num(); i++)
	{
		IRTSCameraFadeInterface::Execute_SetMaterial(FadeTarget, FadeMesh, i, OriginalMaterials[i]);
	}

	FadeMesh = nullptr;
}

void ARTSCameraPawn::SetArmLength_Implementation(float InZoomAmount)
{

	ZoomAlpha += GetWorld()->GetDeltaSeconds();
	if (ZoomAlpha > 1)
	{
		ZoomAlpha = 1.f;
	}
	if (bZoomSpringArm && bZoom)
	{
		if (bUseCurves)
		{
			if (!ensure(CurveZoomArmLength != nullptr)) return;

			float TargetValueLocal = FMath::Lerp(CurveZoomArmLength->GetFloatValue(TimeLastFrame), CurveZoomArmLength->GetFloatValue(InZoomAmount), ZoomAlpha);
			TimeLastFrame = InZoomAmount;

			if (bDoAdvanceCollisionTest)
			{
				TargetValueLocal = FMath::Clamp(TargetValueLocal, SpringArmZoomIn, ClampedSpringArmMax);
			}

			CameraSpringArm->TargetArmLength = TargetValueLocal; // clamp here
		}
		else
		{
			float ValueLocal = FMath::Lerp(SpringArmZoomOut, SpringArmZoomIn, InZoomAmount);

			if (bDoAdvanceCollisionTest)
			{
				ValueLocal = FMath::Clamp(ValueLocal, SpringArmZoomIn, ClampedSpringArmMax);
			}

			CameraSpringArm->TargetArmLength = ValueLocal;  // clamp here
		}
	}

}

void ARTSCameraPawn::SetArmRotation_Implementation(float InZoomAmount)
{
	if (bZoomRotation && bZoom)
	{
		if (bUseCurves)
		{
			if (!ensure(CurveZoomRotation != nullptr)) return;

			float NewPitchLocal = FMath::Lerp(CurveZoomRotation->GetFloatValue(TimeLastFrame), CurveZoomRotation->GetFloatValue(InZoomAmount), ZoomAlpha);
			const FRotator NewRotationLocal = (FRotator(NewPitchLocal, 0.f, 0.f));
			CameraSpringArm->SetRelativeRotation(NewRotationLocal);
		}
		else
		{
			float ValueLocal = FMath::Lerp(PitchZoomOut, PitchZoomIn, InZoomAmount);
			const FRotator NewRotationLocal = (FRotator(ValueLocal, 0.f, 0.f));
			CameraSpringArm->SetRelativeRotation(NewRotationLocal);
		}
	}
}

void ARTSCameraPawn::SetFieldOfView_Implementation(float InZoomAmount)
{
	if (bZoomFOV && bZoom)
	{
		if (bUseCurves)
		{
			if (!ensure(CurveZoomFOV != nullptr)) return;

			const float NewFieldOfViewLocal = FMath::Lerp(CurveZoomFOV->GetFloatValue(TimeLastFrame), CurveZoomFOV->GetFloatValue(InZoomAmount), ZoomAlpha);
			PlayerCamera->SetFieldOfView(NewFieldOfViewLocal);
		}
		else
		{

			float ValueLocal = FMath::Lerp(FieldOfViewZoomIn, FieldOfViewZoomOut, InZoomAmount);
			PlayerCamera->SetFieldOfView(ValueLocal);
		}
	}
}




void ARTSCameraPawn::SetShouldAdjustHeight_Implementation(bool ShouldAdjustPawnHeight)
{
	bAdjustPawnHeightForTerrain = ShouldAdjustPawnHeight;
}

void ARTSCameraPawn::SetUseLandscapeOffset_Implementation(bool UseOffset)
{
	bUseLandscapeOffset = UseOffset;

	AdjustDesiredHeightToTarget();
}

void ARTSCameraPawn::SetLandscapeOffset_Implementation(float Offset)
{
	LandscapeZ = Offset;

	AdjustDesiredHeightToTarget();
}

void ARTSCameraPawn::SetTargetHeight_Implementation(float Target)
{
	DesiredCameraHeight = Target;

	AdjustDesiredHeightToTarget();
}

void ARTSCameraPawn::SetCameraHeightTolerance_Implementation(float Kappa)
{
	CameraHeightTolerance = Kappa;
}

void ARTSCameraPawn::AdjustDesiredHeightToTarget_Implementation()
{
	if (bUseLandscapeOffset)
	{

		TargetCameraHeight = DesiredCameraHeight - LandscapeZ;
	}
	else
	{
		TargetCameraHeight = DesiredCameraHeight;
	}
}

void ARTSCameraPawn::SetCanFadeObjects_Implementation(bool CanFade)
{
	bRunFadeCheck = CanFade;
}

void ARTSCameraPawn::SetFadeMaterial_Implementation(UMaterialInterface* NewMaterial)
{
	if (!ensure(NewMaterial != nullptr)) return;

	FadeMaterial = NewMaterial;
}

void ARTSCameraPawn::SetFadeTraceDistance_Implementation(float Distance)
{
	FadeTraceDistance = Distance;
}


void ARTSCameraPawn::SetCanZoom_Implementation(bool CanZoom)
{
	bZoom = CanZoom;
}

bool ARTSCameraPawn::SetUsesCurves_Implementation(bool UsesCurves)
{
	if (UsesCurves)
	{
		if (!CurveZoomArmLength || CurveZoomRotation || CurveZoomFOV)
		{
			bUseCurves = false;
			return false;
		}
		else
		{
			bUseCurves = UsesCurves;
			return true;
		}
	}
	else
	{
		bUseCurves = UsesCurves;
		return true;
	}
}

void ARTSCameraPawn::SetZoomAdjustsSpringArm_Implementation(bool AdjustSpringArm)
{
	bZoomSpringArm = AdjustSpringArm;
}

void ARTSCameraPawn::SetZoomAdjustsRotation_Implementation(bool AdjustRotation)
{
	bZoomRotation = AdjustRotation;
	if (bZoomRotation)
	{
		SetPanAdjustsRotation(false);
	}

}

void ARTSCameraPawn::SetZoomAdjustsFOV_Implementation(bool AdjustFOV)
{
	bZoomFOV = AdjustFOV;
}

void ARTSCameraPawn::SetPanAdjustsRotation_Implementation(bool AdjustRotation)
{
	bRotateOnPan = AdjustRotation;
	if (bRotateOnPan)
	{
		SetZoomAdjustsRotation(false);
	}
}

void ARTSCameraPawn::SetPanSpeed_Implementation()
{
	// This is NOT a wrapper, because, the code is ensuring that the Edited Default Settings are NOT violating the min and max. 
	// @TODO consider making this a wrapper to UpdatePanSpeed(PanSpeedSettings.Current); 
	// The above idea should achieve the same effect as the code below. 
	// REGARDLESS IF YOU USE THE ABOVE IDEA, keep the last line of this function.
	PanSpeedSettings.Current = FMath::Clamp(PanSpeedSettings.Current, PanSpeedSettings.Min, PanSpeedSettings.Max);
	PanSpeed = PanSpeedSettings.Current;
}

void ARTSCameraPawn::UpdatePanSpeed_Implementation(float NewSpeed)
{
	PanSpeedSettings.Current = FMath::Clamp(NewSpeed, PanSpeedSettings.Min, PanSpeedSettings.Max);
	PanSpeed = PanSpeedSettings.Current;
}

void ARTSCameraPawn::SetMovementSpeed_Implementation()
{
	// used on begin play to ensure that the default settings are within min and max values
	// Then sets the default speed (used for restoring movement speed)
	UpdateMovementSpeed(MovementSpeedSettings.Current); // SHOULD clamp the value
}

void ARTSCameraPawn::UpdateMovementSpeed_Implementation(float NewSpeed)
{
	MovementSpeedSettings.Current = FMath::Clamp(NewSpeed, MovementSpeedSettings.Min, MovementSpeedSettings.Max);
	PawnMovementComponent->SetDefaultMovementSpeed(MovementSpeedSettings.Current);
}





void ARTSCameraPawn::CallSetArmlength_Implementation(float Value)
{
	SetArmLength(Value);
}

void ARTSCameraPawn::CallSetArmRotation_Implementation(float Value)
{
	SetArmRotation(Value);
}

void ARTSCameraPawn::CallSetFieldOfView_Implementation(float Value)
{
	SetFieldOfView(Value);
}

void ARTSCameraPawn::CallPanCamera_Implementation(float Value)
{
	FRotator NewRotationLocal = GetActorRotation().Add(0.f, Value, 0.f);
	//CameraSpringArm->SetRelativeRotation(NewRotationLocal);
	SetActorRotation(NewRotationLocal);
}

void ARTSCameraPawn::CallRotateCamera_Implementation(float Value)
{

	float ClampedRotation = FMath::Clamp((CameraSpringArm->GetRelativeRotation().Pitch + Value), MinRotation, MaxRotation);
	//FRotator NewRotationLocal = CameraSpringArm->GetRelativeRotation().Add(ClampedRotation, 0.f, 0.f);
	FRotator NewRotationLocal = FRotator(ClampedRotation, 0.f, 0.f);
	CameraSpringArm->SetRelativeRotation(NewRotationLocal);
}

void ARTSCameraPawn::CallResetRotation_Implementation()
{
	if (bRotateOnPan)
	{
		CameraSpringArm->SetRelativeRotation(DefaultCameraRotation);
	}
}

void ARTSCameraPawn::CallSetDoAdvanceCamerCollision_Implementation(bool NewValue)
{
	CameraSpringArm->SetDoesAdvanceCollisionTest(NewValue);
}

void ARTSCameraPawn::CallGetClampedSpringArmLength_Implementation(float& NewLengthOut)
{
	CameraSpringArm->AdvanedCollisionTest(NewLengthOut);
}

void ARTSCameraPawn::TrackObject_Implementation()
{
	if (bCanTrack)
	{
		FHitResult HitResultsLocal;
		if (PlayerControllerRef->GetHitResultUnderCursorForObjects(ObjectsToTrack, true, HitResultsLocal))
		{
			if (HitResultsLocal.GetActor() != nullptr)
			{
				AttachAsFollowCamera(HitResultsLocal.GetActor());
			}
			else
			{
				if (bIsTracking)
					CloseFollow();
			}
		}
		else
		{
			if (bIsTracking)
				CloseFollow();
		}
	}
}

void ARTSCameraPawn::AttachAsFollowCamera_Implementation(AActor* FollowActor, bool Bypass /*= false */)
{
	if (!ensure(FollowActor != nullptr)) return;

	FollowTargetRef = FollowActor;
	AttachAlpha = 0.f;
	bIsAttaching = true;
	GetWorld()->GetTimerManager().SetTimer(CameraAttachTimerHandle, this, &ARTSCameraPawn::AttachTimer, TrackAttachSpeed, true);

}

void ARTSCameraPawn::AttachTimer_Implementation()
{
	if (FollowTargetRef)
	{
		FVector RawTargetLocationLocal = FMath::Lerp(GetActorLocation(), FollowTargetRef->GetActorLocation(), AttachAlpha);
		SetActorLocation(RawTargetLocationLocal);
		AttachAlpha += TrackAttachSpeed;
		if (AttachAlpha > 1.f)
		{
			GetWorld()->GetTimerManager().ClearTimer(CameraAttachTimerHandle);
			CameraAttachTimerHandle.Invalidate();
			OpenFollow();
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(CameraAttachTimerHandle);
		CameraAttachTimerHandle.Invalidate();
		bIsAttaching = false;
		bIsTracking = false;
	}
}

void ARTSCameraPawn::OpenFollow_Implementation()
{
	if (!PawnMovementComponent->IsEdgeScrollAlwaysDisabled())
	{
		PawnMovementComponent->SetDisableEdgeScroll(true);
	}
	PawnMovementComponent->SetCameraDisabled(true);
	bIsAttaching = false;
	bIsTracking = true;
}

void ARTSCameraPawn::CloseFollow_Implementation()
{
	if (!PawnMovementComponent->IsEdgeScrollAlwaysDisabled())
	{
		PawnMovementComponent->SetDisableEdgeScroll(false);
	}
	PawnMovementComponent->SetCameraDisabled(false);
	bIsTracking = false;
	if (FollowTargetRef)
	{
		FollowTargetRef = nullptr;
	}
	if (!bAdjustPawnHeightForTerrain)
	{
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetDesiredCameraHeight()), false);
	}
}

void ARTSCameraPawn::RunFollow_Implementation()
{
	if (FollowTargetRef)
	{
		SetActorLocation(FollowTargetRef->GetActorLocation());
	}
}

void ARTSCameraPawn::ForceBreakFollow_Implementation()
{
	if (bIsTracking)
	{
		CloseFollow();
	}
}

void ARTSCameraPawn::SetCanTrack_Implementation(bool CanTrack, bool DisableCurrentTracking /*= true*/)
{
	bCanTrack = CanTrack;

	if (DisableCurrentTracking)
	{
		CloseFollow();
	}
}

void ARTSCameraPawn::PushMovementSpeed_Implementation(float NewSpeed)
{
	UpdateMovementSpeed(NewSpeed);
}

void ARTSCameraPawn::PushPanSensitivity_Implementation(float NewPanSensitivity)
{
	UpdatePanSpeed(NewPanSensitivity);
}

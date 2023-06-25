// Camera Pawn, written by Zachary J. Parker (2021), adapted from earlier work.
// Camera pawn code may be used for this project and has been previously used by the above in a tutorial.

#define LANDSCAPEChannel ECC_GameTraceChannel2

#include "Core/Player/CameraPawn.h"
#include "Math/Rotator.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"

// Sets default values
ACameraPawn::ACameraPawn()
{
	// Set siz of root component (collision sphere)
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(CollisionSphere);
	CollisionSphere->InitSphereRadius(32.0);
	CollisionSphere->SetWorldScale3D(FVector(0.25, 0.25, 0.25));
	CollisionSphere->SetCollisionProfileName("CameraPawn");
	bReplicates = true;

	// Default settings for inheriting controller rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create Camera Boom (spring arm) and attach it to root (collission sphere)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(false);
	// CameraBoom->bAbsoluteRotation = false; // Rotate arm (and camera) when pawn rotates
	CameraBoom->TargetArmLength = DefaultZoomDistance; // this is also the default zoom length
	CameraBoom->SetRelativeRotation(DefaultCameraRotation); // Pitch (Y), Yaw (Z), Roll (X)
	CameraBoom->bDoCollisionTest = false; // DO NOT DO COLLISION TEST ON THE SPRING ARM!
	CameraBoom->bEnableCameraLag = true; // makes for smoother movement
	CameraBoom->CameraLagSpeed = 10.0f;
	CameraBoom->bEnableCameraRotationLag = true; // lag is usful for preventing 'jerking' motion
	CameraBoom->CameraRotationLagSpeed = 10.0f;
	CameraBoom->bInheritPitch = false; // needed to ensure zoom is updated in real time.

	//create Camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	PlayerCamera->FieldOfView = DefaultFieldOfView;
	

	//Attach Movement Component to Camera Pawn
	PawnMovementComponent = CreateDefaultSubobject<UCameraMovementComponent>(TEXT("PawnMovementComponent"));

	//Find (cast, hard reference and is needed here) to the curves
	FSoftObjectPath CurveZoomArmLengthPath(TEXT("/Game/Core/Player/Curve_ZoomArmLength")); // stream in spring arm curve
	UObject* CurveZoomArmLengthRef = CurveZoomArmLengthPath.TryLoad(); // load spring arm curve
	CurveZoomArmLength = Cast<UCurveFloat>(CurveZoomArmLengthRef); // apply loaded curve

	FSoftObjectPath CurveZoomRotationPath(TEXT("/Game/Core/Player/Curve_ZoomRotation")); // stream in rotation curve
	UObject* CurveZoomRotationRef = CurveZoomRotationPath.TryLoad(); // load spring rotation curve
	CurveZoomRotation = Cast<UCurveFloat>(CurveZoomRotationRef); // apply rotation curve

	FSoftObjectPath CurveZoomFieldOfViewPath(TEXT("/Game/Core/Player/Curve_ZoomFOV")); // stream in field of view curve
	UObject* CurveZoomFOVRef = CurveZoomFieldOfViewPath.TryLoad(); // load field of view curve
	CurveZoomFOV = Cast<UCurveFloat>(CurveZoomFOVRef); // apply field of view curve
	
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	//PlayerCamera.bIsCameraMoveableWhenPaused = true;
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AdjustCameraHeight(DeltaTime);

}

// METHODS Definitions
void ACameraPawn::SetArmLength(float InZoomAmount)
{
	if (CurveZoomArmLength != nullptr)
	{
		CameraBoom->TargetArmLength = CurveZoomArmLength->GetFloatValue(InZoomAmount);
	}
}

void ACameraPawn::SetArmRotation(float InZoomAmount)
{
	if (CurveZoomRotation != nullptr)
	{
		const float NewPitchLocal = CurveZoomRotation->GetFloatValue(InZoomAmount);
		const FRotator NewRotationLocal = (FRotator(NewPitchLocal, 0, 0));
		CameraBoom->SetRelativeRotation(NewRotationLocal);
	}
}

void ACameraPawn::SetFieldOfView(float InZoomAmount)
{
	if (CurveZoomFOV != nullptr)
	{
		const float NewFieldOfViewLocal = CurveZoomFOV->GetFloatValue(InZoomAmount);
		PlayerCamera->SetFieldOfView(NewFieldOfViewLocal);
/*#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Current FOV: %F"), GetCurrentFieldOfView());
#endif*/
	}
}

void ACameraPawn::CallSetArmlength_Implementation(float Value)
{
	SetArmLength(Value);
}

void ACameraPawn::CallSetArmRotation_Implementation(float Value)
{
	SetArmRotation(Value);
}

void ACameraPawn::CallSetFieldOfView_Implementation(float Value)
{
	SetFieldOfView(Value);
}

void ACameraPawn::AdjustCameraHeight(float DeltaTime)
{
	// TODO: add lerp on the adjustments below.

	// determine start and end points for line trace
	FVector StartLocationLocal = GetActorLocation();
	FVector EndLocationLocal = GetActorLocation() - FVector(0.0f, 0.0f, 100000.0f);
	

	// declare results
	FHitResult OutHitLocal;

	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = false;
	CollisionParams.AddIgnoredActor(this);
	

	// Run line trace.
	GetWorld()->LineTraceSingleByChannel(OutHitLocal, StartLocationLocal, EndLocationLocal, LANDSCAPEChannel, CollisionParams);

	// Check if at correct height - runs if pawn needs to be adjusted
	float DistanceLocal = FVector::Dist(OutHitLocal.ImpactPoint, GetActorLocation());
	if (!FMath::IsNearlyEqual(DistanceLocal, 189.0f, 0.1f))
	{
		AdjustCameraAlpha += DeltaTime;
		if (AdjustCameraAlpha >= 1)
			AdjustCameraAlpha = 1;
		// Check is pawn needs to move up or down.
		if (DistanceLocal >= 189.0f)
		{
			FVector NewLocationLocal = GetActorLocation() - FVector(0.0f, 0.0f, DistanceLocal - 189.0f);
			SetActorLocation(FMath::Lerp(GetActorLocation(), NewLocationLocal, AdjustCameraAlpha), true); // TODO LERP HERE! (perhaps removenullptr, ETeleportType::ResetPhysics)
		}
		else if (DistanceLocal < 189.0f) // TODO IT IS NOT THE LERP! IT IS NOT FIGHTING WITH THE MOVEMENT CONTROLS! IT IS SELF-FIGHTING
			// THE ABOVE (if-statement) IS BEING TRIGGERED BY THE BELOW (else-if) STATEMENT
		{
			FVector NewLocationLocal = GetActorLocation() + FVector(0.0f, 0.0f, 290.0f);
			SetActorLocation(FMath::Lerp(GetActorLocation(), NewLocationLocal, AdjustCameraAlpha), true); // TODO LERP HERE! (Maybe remove lerp?)
		}
	}
	else
	{
		AdjustCameraAlpha = 0.0f;
	}
}
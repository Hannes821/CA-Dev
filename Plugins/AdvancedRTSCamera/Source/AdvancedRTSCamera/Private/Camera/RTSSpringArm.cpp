// Copyright Two Neurons Studio, LLC. Year of Publication 2024.


#include "Camera/RTSSpringArm.h"


URTSSpringArm::URTSSpringArm(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.bTraceComplex = false;
}

void URTSSpringArm::AdvanedCollisionTest(float& NewLengthOut)
{
	FHitResult Hit;
	if (AdvancedCollisionTrace(Hit))
	{
		ImpactPoint = Hit.ImpactPoint;
		NewLengthOut = CalculateAdjustedArmLength();
	}
	else
	{
		if (ShouldRestoreArmLength())
		{
			NewLengthOut = CalculateRestoreTargetArmlength();
		}
	}
}


bool URTSSpringArm::AdvancedCollisionTrace(FHitResult& HitOut)
{
	const FVector Start = GetComponentLocation();

	FVector NormalizedCameraLocation = ((CameraLocation + SocketOffset + TargetOffset) - Start);
	NormalizedCameraLocation.GetSafeNormal(0.0001); // @TODO double check.
	NormalizedCameraLocation * ExpectedSpringArmLength;

	const FVector End = Start + NormalizedCameraLocation;

	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(ProbeRadius);


	return GetWorld()->SweepSingleByChannel(HitOut, Start, End, FQuat::Identity, ECC_Camera, CollisionShape, QueryParams);
}

float URTSSpringArm::CalculateRestoreTargetArmlength()
{
	return FMath::FInterpTo(TargetArmLength, ExpectedSpringArmLength, GetWorld()->GetDeltaSeconds(), GetInterpSpeed());;
}

void URTSSpringArm::SetDoesAdvanceCollisionTest(bool Value)
{
	bDoAdanceCollisionTest = Value;
}

void URTSSpringArm::SetSpringArmLengthMin(float MinLength)
{
	SpringArmMinLength = MinLength;
}

void URTSSpringArm::SetSpringArmLengthMax(float MaxLength)
{
	SpringArmMaxLength = MaxLength;
}

void URTSSpringArm::SetExpectedSpringArmLength(float CurrentExpectedLength)
{
	ExpectedSpringArmLength = CurrentExpectedLength;
}

void URTSSpringArm::GetCameraLocation(FVector NewLocation)
{
	CameraLocation = NewLocation;
}

void URTSSpringArm::SetProbeRadius(float NewRadius)
{
	ProbeRadius = NewRadius;
}
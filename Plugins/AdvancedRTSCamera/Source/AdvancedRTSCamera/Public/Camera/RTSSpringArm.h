// Copyright Two Neurons Studio, LLC. Year of Publication 2024.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "RTSSpringArm.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (CameraRTS))
class ADVANCEDRTSCAMERA_API URTSSpringArm : public USpringArmComponent
{
	GENERATED_BODY()

	friend class ARTSCameraPawn;

/************************************************
* ATTRIBUTES
************************************************/
private:
	/* unexposed, to set this variable, find the equivilant in the custom movement component and set there */
	bool bDoAdanceCollisionTest{ false };
	/* Unexposed. Do not edit. It is set and controlled by the pawn and movement component*/
	float ExpectedSpringArmLength{ 3500.f };
	/* The spring arm length as a result of the collision. Do not edit. */
	float AdjustedSpringArmLength{ 3500.f };
	/* Do not edit, outside of safety reasons. This value is set by the pawn on begin play 
	(if you do want to edit for safety reasons, use the smallest value for your spring arm length on zoom in)*/
	float SpringArmMinLength{ 300.f };
	float SpringArmMaxLength{ 3500.f };
	/* do not edit updated on tick by component*/
	FVector CameraLocation;
	/* never touch - do not edit. This is set by this class */
	FVector ImpactPoint;
	/* Parameters for collision tests for advanced collison test */
	FCollisionQueryParams QueryParams;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AdvanceCollisionTest", meta = (AllowPrivateAccess = "true"))
	float ProbeRadius{ 20.f };
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AdvanceCollisionTest", meta = (AllowPrivateAccess = "true"))
	float InterpSpeed{ 5.f };
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AdvanceCollisionTest", meta = (AllowPrivateAccess = "true"))
	float InterpSpeedModifier{ 10.f };


/************************************************
* METHODS
************************************************/
	/************************************************
	* INHERITED METHODS/CONSTRUCTORS
	************************************************/
	URTSSpringArm(const FObjectInitializer& ObjectInitializer);

	/************************************************
	* MEMBER METHODS
	************************************************/
private:
	void AdvanedCollisionTest(float& NewLengthOut);
	bool AdvancedCollisionTrace(FHitResult& HitOut);

	FORCEINLINE float GetInterpSpeed() const 
	{
		float Distance = FVector::Distance(CameraLocation, ImpactPoint);
		return FMath::GetMappedRangeValueClamped(FVector2D(SpringArmMinLength, SpringArmMaxLength), FVector2D(InterpSpeed, InterpSpeed * InterpSpeedModifier), Distance);
	}
	
	FORCEINLINE float CalculateAdjustedArmLength() const { 
		float Distance = FVector::Distance(GetComponentLocation(), ImpactPoint);
		float Target = Distance - ProbeRadius;
		return FMath::FInterpTo(TargetArmLength, Target, GetWorld()->GetDeltaSeconds(), GetInterpSpeed());
	}

	float CalculateRestoreTargetArmlength();
	FORCEINLINE bool ShouldRestoreArmLength() const { return !FMath::IsNearlyEqual(TargetArmLength, ExpectedSpringArmLength, 0.1); }


public:
	void SetDoesAdvanceCollisionTest(bool Value); // @TODO - make the equivilant movement comp bp accessible - so that dynamic switching of this mode can be achieved.
	void SetSpringArmLengthMin(float MinLength);
	void SetSpringArmLengthMax(float SpringArmMaxLength);
	void SetExpectedSpringArmLength(float CurrentExpectedLength);
	void GetCameraLocation(FVector NewLocation);

	UFUNCTION(BlueprintCallable, Category = "AdvanceCollisionTest")
	void SetProbeRadius(float NewRadius);
	

	
	UFUNCTION(Blueprintpure, BlueprintCallable, Category = "AdvanceCollisionTest")
	FORCEINLINE bool GetDoesAdanceCollisionTest() const { return bDoAdanceCollisionTest; }
	UFUNCTION(Blueprintpure, BlueprintCallable, Category = "AdvanceCollisionTest")
	FORCEINLINE float GetAdjustedSpringArmLength() const { return AdjustedSpringArmLength; }

};

// Camera Pawn, written by Zachary J. Parker (2021), adapted from earlier work.
// Camera pawn code may be used for this project and has been previously used by the above in a tutorial.
#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "Core/Components/CameraMovementComponent.h"
#include "Core/Interfaces/CameraPawnInterface.h"
#include "CameraPawn.generated.h"

//class UCameraMovementComponent;

UCLASS()
class ACameraPawn : public APawn, public ICameraPawnInterface
{
	GENERATED_BODY()

/************************************************
* CONSTRUCTORS
************************************************/
public: 
	// Sets default values for this pawn's properties
	ACameraPawn();

/************************************************
* ATTRIBUTES
************************************************/
public:
	/** Custom movement component for player camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Controls", meta = (AllowPrivateAcces = "true"))
	class UCameraMovementComponent* PawnMovementComponent;

protected:
	/** Collision Sphere and root component of camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root Component")
	class USphereComponent* CollisionSphere; // This sphere is used to test for collisions with boundaries of the map; and for anchoring everything to.

	/** Player Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* PlayerCamera;

	/** Camera Boom, used for zoom */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	/** Default CameraBoom Length */
	UPROPERTY(Category = " Movement Variables|Zoom", BlueprintReadOnly)
	float DefaultZoomDistance{ 1350.0f };

	/** Default Pawn Rotation */
	UPROPERTY(Category = " Movement Variables|Zoom", BlueprintReadOnly)
	FRotator DefaultCameraRotation { -75.0f, 0.0f, 0.0f };

	/** Default Field of View */
	UPROPERTY(Category = " Movement Variables|Zoom", BlueprintReadOnly)
	float DefaultFieldOfView{ 90.0f };

	/** Curve for CameraBoom */
	UPROPERTY(Category = " Movement Variables|Zoom", BlueprintReadOnly)
	UCurveFloat* CurveZoomArmLength;

	/** Curve for Zoom Rotation */
	UPROPERTY(Category = " Movement Variables|Zoom", BlueprintReadOnly)
	UCurveFloat* CurveZoomRotation;

	/** Curve for Zoom Field of View */
	UPROPERTY(Category = " Movement Variables|Zoom", BlueprintReadOnly)
	UCurveFloat* CurveZoomFOV;

private:
	float AdjustCameraAlpha{ 0.0f };
/************************************************
* METHODS
************************************************/
protected: 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/************************************************
	* SETTERS
	************************************************/
	/** Sets CameraBoom length (spring arm length) for zooming */
	UFUNCTION(BlueprintCallable, Category = "Camerea|Zoom")
	void SetArmLength(float InZoomAmount);

	/** Sets CameraBoom rotation for zooming */
	UFUNCTION(BlueprintCallable, Category = "Camerea|Zoom")
	void SetArmRotation(float InZoomAmount);

	/** Sets Field of View for zooming */
	UFUNCTION(BlueprintCallable, Category = "Camerea|Zoom")
	void SetFieldOfView(float InZoomAmount);

	UFUNCTION(BlueprintCallable, Category = "Camera|Height")
	void AdjustCameraHeight(float DeltaTime);

	/************************************************
	* INTERFACE RELATED
	************************************************/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface|Zoom")
	void CallSetArmlength(float Value);
	virtual void CallSetArmlength_Implementation(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface|Zoom")
	void CallSetArmRotation(float Value);
	virtual void CallSetArmRotation_Implementation(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface|Zoom")
	void CallSetFieldOfView(float Value);
	virtual void CallSetFieldOfView_Implementation(float Value);

	/************************************************
	* GETTERS
	************************************************/
	/** Returns Camera */
	FORCEINLINE class UCameraComponent* GetCamera() const { return PlayerCamera; }

	/** Return Camera Boom */
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Reutn length of CameraBoom (spring arm length) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camerea|Zoom")
	FORCEINLINE float GetCurrentArmLength() const { return CameraBoom->TargetArmLength; }

	/** Reutn rotation of CameraBoom */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camerea|Zoom")
	FORCEINLINE FRotator GetCurrentRotation() const { return CameraBoom->GetRelativeRotation(); }

	/** Reutn current Field of View */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camerea|Zoom")
	FORCEINLINE float GetCurrentFieldOfView() const { return PlayerCamera->FieldOfView; };
};

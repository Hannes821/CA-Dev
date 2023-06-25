// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Misc/DateTime.h"
#include "Engine/DirectionalLight.h"
#include "Core/Interfaces/GameTimeInterface.h"
#include "SunRotationBase.generated.h"

/*
* This class will be used to control the rotation of the sun in the game
*/

UCLASS()
class ASunRotationBase : public AActor, public IGameTimeInterface
{
	GENERATED_BODY()
	
/************************************************
* CONSTRUCTORS
************************************************/
public:	
	// Sets default values for this actor's properties
	ASunRotationBase();

/************************************************
* ATTRIBUTES
************************************************/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Date And time")
	FDateTime DateTimeStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	ADirectionalLight* SunRef;

	UPROPERTY(ReplicatedUsing = OnRep_SunRotation)
	FRotator SunRotation;

/************************************************
* METHODS
************************************************/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/* Converst game time struct into pure game time (float) */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ConvertTime(FDateTime TimeStruct);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FRotator CalculateSunRotation(float GameTime);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Sun Rotation")
	void MC_RotateSun();
	void MC_RotateSun_Implementation();

	UFUNCTION()
		void OnRep_SunRotation();

	// Interface events
	/* Gets the Date Time */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SetGameDateTime(FDateTime TimeStruct); // prototype
	virtual bool SetGameDateTime_Implementation(FDateTime TimeStruct);
};

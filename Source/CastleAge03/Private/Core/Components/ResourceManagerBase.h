// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Structs/FResourceStructs.h"
#include "Core/Enums/ResourcesEnum.h"
#include "ResourceManagerBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UResourceManagerBase : public UActorComponent
{
	GENERATED_BODY()

	/************************************************
	* CONSTRUCTORS
	************************************************/
public:	
	// Sets default values for this component's properties
	UResourceManagerBase();

	/************************************************
	* ATTRIBUTES
	************************************************/
protected:
	/************************************************
	* Master
	************************************************/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Resources|Master")
	FResources PlayerResources;

private:
	UPROPERTY(Replicated)
	float Remainder{ 0.0 };

	UPROPERTY(Replicated)
	bool bHasRemainder{ false };

	/************************************************
	* METHODS
	************************************************/
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Sets <Value> resource amount to clamp (current + <Amount>, Max) */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SetResourceAmount(EResources Value, float Amount);
	void ROS_SetResourceAmount_Implementation(EResources Value, float Amount);

	/* Sets <Value> resource max storage to max + <amount> */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SetMaxResourceAmount(EResources Value, float Amount);
	void ROS_SetMaxResourceAmount_Implementation(EResources Value, float Amount);
	
	/* Compare <value> resource to <amount> and return true if equal to or greater than <amount> */
	UFUNCTION(BlueprintCallable)
	bool CompareResource(EResources Value, float Amount);
	
	/* Returns the current amount of <Value> resources */
	UFUNCTION(BlueprintCallable)
	float GetCurrentResourceAmount(EResources Value);

	/* Returns the current <value> Max*/
	UFUNCTION(BlueprintCallable)
	float GetMaxResourceAmount(EResources Value);
};

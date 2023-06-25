// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Core/Actors/ResourceMasterBase.h"
#include "ResourceSpawnerBase.generated.h"

/*
* This class will control what resources are procedurally spawned
* Designers can set particular values in editor
* A timer will be added in to respawn missing resources. 
* Timer will be linked to game speed (using new system).
*/

UCLASS()
class AResourceSpawnerBase : public AActor
{
	GENERATED_BODY()
	
/************************************************
* CONSTRUCTORS
************************************************/
public:	
	// Sets default values for this actor's properties
	AResourceSpawnerBase();

/************************************************
* ATTRIBUTES
************************************************/
protected:
	/************************************************
	* Components
	************************************************/
	/* Spawning Volume for resources */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* SpawningVolume;
	/************************************************
	* Master
	************************************************/
	/* Reference to the datatable used to run this.*/
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Master|Do Not Edit")
	UDataTable* ResourceDataTable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Master")
	TArray<FName> ResourceNames;

	/* Flagged for removal */
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Master|Do Not Edit")
	TArray<FName> FlaggedNames;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated, Category = "Master|Settings")
	bool bRefreshSettings{ true };

private:
	UPROPERTY(Replicated)
	bool bSpawnerIsValid{ false };

	TSubclassOf<AResourceMasterBase> ResourceToSpawn;


/************************************************
* METHODS
************************************************/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_CheckSpawnerValidity();
	virtual void ROS_CheckSpawnerValidity_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_StartSpawner(bool bIsValid);
	virtual void ROS_StartSpawner_Implementation(bool bIsValid);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SpawnResource(FName RowName, FResourceStruct ResourceDetails);
	virtual void ROS_SpawnResource_Implementation(FName RowName, FResourceStruct ResourceDetails);

	bool CheckAllowedSurfaceTypes(FHitResult& Results, FAllowedSpawnLocations AllowedLocations);

	FRotator CalculateRotation(FMInMaxStruct X, FMInMaxStruct Y, FMInMaxStruct Z);

	FVector CalculateScale(TEnumAsByte <EScaleSettings> Setting, FMInMaxStruct X, FMInMaxStruct Y, FMInMaxStruct Z);
};

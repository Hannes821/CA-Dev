// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Core/Enums/CommonEnums.h"
#include "Core/Structs/FResourceStructs.h"
#include "Core/Interfaces/GameTimeInterface.h"
#include "ResourceMasterBase.generated.h"

/*
* This is the master for an individual resource
*/

UCLASS()
class AResourceMasterBase : public AActor, public IGameTimeInterface
{
	GENERATED_BODY()

/************************************************
* CONSTRUCTORS
************************************************/
public:	
	// Sets default values for this actor's properties
	AResourceMasterBase();

/************************************************
* ATTRIBUTES
************************************************/
protected:
	/* The root for the Resource.*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "Master")
	USceneComponent* ResourceRoot;

	/* The mesh currently loaded in.*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "Master")
	UStaticMeshComponent* ResourceMesh;

	/* The overall collision box for the building.*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "Master")
	UBoxComponent* HarvestArea;

	/* Reference to the datatable used to run this.*/
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Master|Do Not Edit")
	UDataTable* ResourceDataTable;

	/* The RowName of the Resource, enter a valid value to make this class work*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Master", Meta = (ExposeOnSpawn = "true"))
	FName RowName {	"Null"	};

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Master")
	FResourceStruct ResourceData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Master")
	bool bAdjustCollisionOffset{false};
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Master")
	FVector CustomCollisionSize{1.75f, 1.75f, 1.0f};
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Master|Do Not Edit")
	TEnumAsByte<ESeasons> Season;


/************************************************
* METHODS
************************************************/
protected:
	// construction scripts
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void ROS_SetSeason();
	void ROS_SetSeason_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SetResourceData();
	void ROS_SetResourceData_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_UpdateMesh();
	void ROS_UpdateMesh_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SetHarvestableArea();
	void ROS_SetHarvestableArea_Implementation();

public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SetRowName(FName Value);
	void ROS_SetRowName_Implementation(FName Value);
};

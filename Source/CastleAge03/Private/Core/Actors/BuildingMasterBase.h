// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Core/Structs/FBuildingStructs.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Core/Interfaces/BuildingInterface.h"
#include "BuildingMasterBase.generated.h"


/*
* This class is the building. Used for controlling all building variables.
*/


UCLASS()
class ABuildingMasterBase : public AActor, public IBuildingInterface
{
	GENERATED_BODY()
	
/************************************************
* CONSTRUCTORS
************************************************/
public:	
	// Sets default values for this actor's properties
	ABuildingMasterBase();

/************************************************
* ATTRIBUTES
************************************************/

	/************************************************
	* Master  
	* - needed for buildings to show up and basic functions
	************************************************/
private:
	UPROPERTY(Replicated)
	TEnumAsByte<EConstructionStatus> ConstructionStatus{ Built };

	UPROPERTY(ReplicatedUsing = OnRep_UpdateBuildingStatus)
	int BuildingStatus{ -1 };

	UPROPERTY(ReplicatedUsing = OnRep_IsDestroyed)
	bool bIsDestroyed{ false };

	// CHANGE IF YOU CHANGE IN GAMESTATE!
//	float GameSpeed{ 60.0f };


//	float SpeedMutlipler{ 1.0f };

protected:
	/* The structure for this particular building*/
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Master")
	FBuildingStruct BuildingData;

	/* The RowName of the Building, enter a valid value to make this class work*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Master", Meta = (ExposeOnSpawn = "true"))
	FName RowName { "Null" };

	/* The root for the building.*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "Master")
	USceneComponent* BuildingRoot;

	/* The mesh currently loaded in.*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "Master")
	UStaticMeshComponent* CurrentBuildingMesh;

	/* The overall collision box for the building.*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "Master")
	UBoxComponent* CollisionBox;

	/* The front of the building where units will enter or have to be near.*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "Master")
	UBoxComponent* EntryPoint;

	/* Reference to the datatable used to run this.*/
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Master|Do Not Edit")
	UDataTable* BuildingsDataTable;

	/* Reference to the actor who owns this building */
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Master")
	AActor* BuildingOwnerRef;

	/************************************************
	* Appearance related
	************************************************/

	/* Current Meshes Applied */
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Master")
	TArray<UMaterialInstanceDynamic*> MaterialList;

	/* The ghost material used for previewing the building */
	UMaterialInstance* PreviewMaterial;

	/************************************************
	* Construction related
	************************************************/
	
	/* Bool to determine if building is overlapping during preview stage*/
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Master|Do Not Edit")
	FVector Extent;

	/* Storage for the difference between the contact points and the landscape */
	TArray<float> Differences;

private:
	/* Bool to determine if building is on a slope during preview stage*/
	bool bIsOnSlope{ false };

	/* How far along in being built is this building */
	UPROPERTY(Replicated)
	float ConstructedTime{ 0.0f };



/************************************************
* METHODS
************************************************/
protected:
	// construction scripts
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/************************************************
	* GLOBAL BUILDING RELATED FUNCTIONS
	* The first set of functions below are used to
	* set all buildings regardless of status or owner
	* Though one function sets the owner (net and ref)
	************************************************/

	// Set mesh functions. The replicated versions just call the standard one. 
	// If no index is passed in it will use, by default, the initial index for the meshes. 
	
	/* Server sets mesh */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SetMesh(int Index = 0);
	void ROS_SetMesh_Implementation(int Index = 0);

	/* Used for setting preview version on client only */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_SetMesh(int Index = 0);
	void ROC_SetMesh_Implementation(int Index = 0);

	/* Sets the visible mesh for the building*/
	UFUNCTION(BlueprintCallable)
	void SetMesh(int Index = 0);

	/*Sets the collision box around the building*/
	UFUNCTION(BlueprintCallable)
	void SetCollisionArea();

	/* sets the collision box for the entry point*/
	UFUNCTION(BlueprintCallable)
	void SetEntrance();

	/* sets the owner for the netserver and stores a reference to the player controller.*/
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SetBuildingOwner(APlayerController* OwnerIn);
	virtual void ROS_SetBuildingOwner_Implementation(APlayerController* OwnerIn);

	/* sets if the building construction Status*/
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SetConstructionStatus(EConstructionStatus Status);
	virtual void ROS_SetConstructionStatus_Implementation(EConstructionStatus Status);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SetBuildingStatus(int Value);
	virtual void ROS_SetBuildingStatus_Implementation(int Value);

	UFUNCTION()
	virtual void OnRep_UpdateBuildingStatus();

	/* Updates game time for construction and production*/
	//UFUNCTION(BlueprintCallable)
	//void UpdateGameSpeed(float inGameSpeed);

/************************************************
* PREVIEW/GHOST BUILDING METHODS
************************************************/
	
	/* Destroys the preview mesh */
	UFUNCTION(BlueprintCallable)
	void DestroyPreview();

	/* Sets preview version on client only */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_SetPreviewMesh();
	void ROC_SetPreviewMesh_Implementation();

	/* This sets the preview (ghost) material for all material indexes of the mesh - default is blue - called by BuildManger via interface when constructing a preview*/
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_SetPreviewMaterial();
	void ROC_SetPreviewMaterial_Implementation();

	/* Sets the actor location. The code for what the location is, is passed through the building manager component */
	UFUNCTION(BlueprintCallable)
	void MovePreview(FVector NewLocation);

	/* Sets the actor rotation. The code for what the rotation should be, is passed through the building manager component by the player controller */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_RotatePreview(FRotator NewRotation);
	void ROC_RotatePreview_Implementation(FRotator NewRotation);

	/* Sets the material to red or blue based on if there is an overlap or not, respectively. */
	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void ROC_SetMaterialOverlap();
	void ROC_SetMaterialOverlap_Implementation();

	/* Returns the difference between the building and the landscape*/
	UFUNCTION(BlueprintCallable)
	float SlopeTrace(FVector TraceLocation);

	/* Calculates if mesh is a slope and false if there is not */
	UFUNCTION(BlueprintCallable)
	bool CalculateSlope();

	/************************************************
	* CONSTRUCTION RELATED FUNCTIONS
	************************************************/

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SetConstructionMesh();
	void ROS_SetConstructionMesh_Implementation();

	/************************************************
	* COMBAT AND HEALTH RELATED
	************************************************/

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SetStabilityMin(float NewStability);
	void ROS_SetStabilityMin_Implementation(float NewStability);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SetIsDestroyed(bool Value);
	void ROS_SetIsDestroyed_Implementation(bool Value);

	UFUNCTION()
	virtual void OnRep_IsDestroyed();

	/************************************************
	* INTERFACE RELATED (PREVIEW)
	************************************************/
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerDestroyPreview();
	virtual void TriggerDestroyPreview_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetRowName(FName TargetRowName);
	virtual void TriggerSetRowName_Implementation(FName TargetRowName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetOwner(APlayerController* TargetOwner);
	virtual void TriggerSetOwner_Implementation(APlayerController* TargetOwner);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetPreviewMaterial();
	virtual void TriggerSetPreviewMaterial_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerMovePreview(FVector InNewLocation);
	virtual void TriggerMovePreview_Implementation(FVector InNewLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerRotatePreview(FRotator NewRotation);
	virtual void TriggerRotatePreview_Implementation(FRotator NewRotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetConstructionStatus(EConstructionStatus Status);
	virtual void TriggerSetConstructionStatus_Implementation(EConstructionStatus Status);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerAddConstructedTime(float DeltaConstructionTime);
	virtual void TriggerAddConstructedTime_Implementation(float DeltaConstructionTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetMesh(int Index);
	virtual void TriggerSetMesh_Implementation(int Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetBuildingStatus(int Value);
	virtual void TriggerSetBuildingStatus_Implementation(int Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetBuildingMinStability(float NewStability);
	virtual void TriggerSetBuildingMinStability_Implementation(float NewStability);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerSetIsDestroyed(bool Value);
	virtual void TriggerSetIsDestroyed_Implementation(bool Value);

	/************************************************
	* GETTERS
	************************************************/
public:
	/* Gets the DT Row Name for the building. Here if needed. No plan to use it. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Master")
	FORCEINLINE FName GetRowName() const { return RowName; }

	/* Returns the owner of the building */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Master")
	FORCEINLINE AActor* GetBuildingOwner() const { return BuildingOwnerRef; }

	/* Returns the building data */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Master")
	FORCEINLINE FBuildingStruct GetBuildingData() const { return BuildingData; }
	
	/* A pure function that gets if there are any overlapping actors. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Master") 
	bool CheckIsOverlapping(); // MAYBE WORTH CONSIDERING OBJECTS OUTSIDE OF ACTOR

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Master")
	bool GetIsOnSlope() const { return bIsOnSlope;  }

	/* Returns the construction status */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Master")
	FORCEINLINE TEnumAsByte<EConstructionStatus> GetConstructionStatus() const { return ConstructionStatus; }

	/* Returns the time required to construct of the building */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Master")
	FORCEINLINE float GetConstructionTime() const { return BuildingData.BuildingTime; }

	/* Returns the time spend constructing the building */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Master")
	FORCEINLINE float GetTimeConstructed() const { return ConstructedTime; }

	/* Returns the current building mesh status */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Master")
	FORCEINLINE int GetBuildingStatus() const { return BuildingStatus;  }

	/* Returns the current building health status */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Master")
	FORCEINLINE FMInMaxStruct GetStability() const { return BuildingData.Stability; }

	/* inline functions for loading in materials */
	// Maybe worth considering just adding the instances for the preview to the DT. 
	template <typename ObjClass>
	static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
	{
		if (Path == NAME_None) return nullptr;

		return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), nullptr, *Path.ToString()));
	}

	static FORCEINLINE UMaterialInstance* LoadMaterialFromPath(const FName& Path)
	{
		if (Path == NAME_None) return nullptr;

		return LoadObjFromPath<UMaterialInstance>(Path);
	}

};
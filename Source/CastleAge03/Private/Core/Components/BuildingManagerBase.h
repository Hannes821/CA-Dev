// Code written by Zachary Parker for Feldfeste 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "Core/Actors/BuildingMasterBase.h"
#include "BuildingManagerBase.generated.h"

/*
* This class manages the buildings for players and AI.
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UBuildingManagerBase : public UActorComponent
{
	GENERATED_BODY()

/************************************************
* CONSTRUCTORS
************************************************/
public:	
	// Sets default values for this component's properties
	UBuildingManagerBase();

/************************************************
* ATTRIBUTES
************************************************/
private:
	/************************************************
	* Master
	************************************************/
	/* Relevent player controller that owns this component*/
	APlayerController* ControllerRef;

	/* The current building being looked at, mostly used for preview and start of construction.*/
	class ABuildingMasterBase* BuildingRef;

	/* The RowName of the Building, enter a valid value to make this class work*/
	UPROPERTY(Replicated)
	FName BuildingRowName { "Null" };
	
	/* Transform of the building  that is stored in BuildingRef*/
	UPROPERTY(Replicated)
	FTransform BuildingTransform;

	/* List of buildings that need to be constructed */
	UPROPERTY(Replicated)
	TArray<class ABuildingMasterBase*> ListOfUnderConstructionBuildings;

	/* List of Constructed Buildings */
	UPROPERTY(Replicated)
	TArray<class ABuildingMasterBase*> ListOfBuildings;

	/* Is the player currently looking at a preview (ghost) building? */
	bool bIsPreviewing{ false };

	/* Mouse position for tracking building movement */
	FVector MousePosition;
	
	/* Default value for how many UU/CM the building moves during tracking*/
	int SnapValue{ 100 };

	// CHANGE IF YOU CHANGE IN GAMESTATE!
	UPROPERTY(Replicated)
	float GameSpeed{ 60.0f };

	UPROPERTY(Replicated)
	float SpeedMutlipler{ 1.0f };

	float BaseConstructionSpeed{ 0.1f };

	/************************************************
	* Timers
	************************************************/
	/* Timer used to have the building preview (ghost) track the mouse*/
	FTimerHandle PreviewTimerHandle;

	/* Timer used to control construction, so all construction for player runs on one timer*/
	FTimerHandle ConstructionTimerHandle;


/************************************************
* METHODS
************************************************/
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	/************************************************
	* GLOBAL BUILDING RELATED FUNCTIONS
	************************************************/
protected:
	/* Updates game time for construction and production*/
	UFUNCTION(BlueprintCallable)
	void UpdateGameSpeed(float inGameSpeed);

public:

	/************************************************
	* PREVIEW/GHOST BUILDING METHODS
	*
	* Order of events:
	* Player selects which building on UMG (ToolsMenu) -> UMG gets PlayerController() that then calls this class' SetPreview.
	* SetPreview calls SpawnPreviewBuilding (Creates mesh, calls BuildingMasterBase), TrackMouse (get mouse location from player controller, send location to BuildMasterBase).
	*
	* Secondary options: 
	* Player can trigger the building to rotate via the player controller.
	*
	* Destroy Preview destroys the preview mesh. Can be used for if a player decides to leave construction mode. Or (currently) if the player places a building.
	************************************************/

	/* Sets if the player is in preview mode or not,  called in ROC_SetPreview*/
	void SetIsPreviewing(bool bValue);
	
	/* Gets Mouse Position from controller and sets MousePosition*/
	void SetMousePosition(FVector ControllerMousePosition);

	/* Sets a preview (ghost) version of the constructed building on the client's game. Called by PlayerController via the UMG (ToolsMenu) */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_SetPreview(FName RowName);
	virtual void ROC_SetPreview_Implementation(FName RowName);

	/* Spawns the preview mesh in, calls up the BuildingMasterBase. Is called by ROC_SetPreview */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_SpawnPreviewBuilding(FName RowName);
	virtual void ROC_SpawnPreviewBuilding_Implementation(FName RowName);

	/* Functions that sets Building's Location (calls BuildingMasterBase). Runs on a timer trigged in ROC_SetPreview */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_TrackMouse();
	virtual void ROC_TrackMouse_Implementation();

	/* This roates to the preview mesh to the left (calls BuildingMasterBase). Called by Player Controller. */
	UFUNCTION(BlueprintCallable)
	void RotatePreviewLeft();

	/* This roates to the preview mesh to the right (calls BuildingMasterBase). Called by Player Controller. */
	UFUNCTION(BlueprintCallable)
	void RotatePreviewRight();

	/* Destroys preview on client side. Called by Player Controller when placing a building */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_DestroyPreview();
	virtual void ROC_DestroyPreview_Implementation();

	/************************************************
	* CONSTRUCTION METHODS
	*
	* Order of events:
	* Player PrimaryAction (left click) to place ghost/preview building. Player Controller calls StoreVariables to store the needed information.
	* Then Player Controller destroys the preview (ROC_DestroyPreview).
	* Then Player Controller calls SpawnConstruction and passes variables back in (please see the note inside the ROS_SpawnConstruction)
	* Spawn Construction starts the timers on the relevant player.
	************************************************/
	
	/* Stores current row name and transform, if the delete method is used. */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_StoreVariables(FTransform TargetTransform, FName InRowName);
	virtual void ROC_StoreVariables_Implementation(FTransform TargetTransform, FName InRowName);

	/* Runs the spawn events. */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_RunConstructionStart(FTransform TargetTransform, FName InRowName); // DID NOT WORK. REMOVE
	virtual void ROC_RunConstructionStart_Implementation(FTransform TargetTransform, FName InRowName);

	/* Spawns on the server, so all players can see, a new building. Takes RowName and Transform for spawning */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ROS_SpawnConstruction(FTransform TargetTransform, FName InRowName);
	virtual void ROS_SpawnConstruction_Implementation(FTransform TargetTransform, FName InRowName);
	
	/* Stores building reference in BuildingRef*/
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_SetBuildingRef(ABuildingMasterBase* TargetBuilding);
	virtual void ROC_SetBuildingRef_Implementation(ABuildingMasterBase* TargetBuilding);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_CallSetSatus(ABuildingMasterBase* NewBuilding);
	virtual void ROC_CallSetSatus_Implementation(ABuildingMasterBase* NewBuilding);


	/* Adds newly placed building to the list of unconstructed builldings (ListOfUnderConstructionBuildings) */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_AddToUnconstructedList(ABuildingMasterBase* NewBuilding);
	virtual void ROC_AddToUnconstructedList_Implementation(ABuildingMasterBase* NewBuilding);
	
	/* Removes a destroyed or constructed building from the list of unconstructed buildings (ListOfUnderConstructionBuildings) */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_RemoveToUnconstructedList(ABuildingMasterBase* OldBuilding);
	virtual void ROC_RemoveToUnconstructedList_Implementation(ABuildingMasterBase* OldBuilding);

	/* Triggers Construction Timer or invalidates it */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_SetConstructionTimer();
	virtual void ROC_SetConstructionTimer_Implementation();
	
	/* Timer for construction event */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_RunConstructionEvent();
	virtual void ROC_RunConstructionEvent_Implementation();

	/* Adds newly placed building to the list of unconstructed builldings (ListOfUnderConstructionBuildings) */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_AddToListOfBuildings(ABuildingMasterBase* NewBuilding);
	virtual void ROC_AddToListOfBuildings_Implementation(ABuildingMasterBase* NewBuilding);

	/* Removes a destroyed or constructed building from the list of unconstructed buildings (ListOfUnderConstructionBuildings) */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_RemoveToListOfBuildings(ABuildingMasterBase* DestroyedBuilding);
	virtual void ROC_RemoveToListOfBuildings_Implementation(ABuildingMasterBase* DestroyedBuilding);

	/************************************************
	* COMBAT AND HEALTH RELATED
	*
	************************************************/
	void RunDamageEvent(ABuildingMasterBase* TargetBuilding, float DamageAmount);

	bool DamageBuilding(ABuildingMasterBase* TargetBuilding, float DamageAmount);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ROC_DestroyBuilding(ABuildingMasterBase* TargetBuilding);
	virtual void ROC_DestroyBuilding_Implementation(ABuildingMasterBase* TargetBuilding);

	/************************************************
	* GETTERS
	************************************************/

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsPreviewing() const { return bIsPreviewing; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE class ABuildingMasterBase* GetPreviewBuilding() const { return BuildingRef; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FVector GetMousePosition() const { return MousePosition; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetGridSnaps();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FName GetRowName() const { return BuildingRowName; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FTransform GetTargetTransform() const { return BuildingTransform; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<class ABuildingMasterBase*> GetListOfUnconstructedBuildings() const { return ListOfUnderConstructionBuildings; }

};

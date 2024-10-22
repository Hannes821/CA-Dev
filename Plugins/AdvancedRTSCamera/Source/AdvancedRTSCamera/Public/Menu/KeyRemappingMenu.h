// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMappableInputConfig.h"
#include "Kismet/KismetInputLibrary.h"
#include "OptionsMenuMaster.h"
#include "IndividualKeyMap.h"
#include "KeyMappingStructs.h"
#include "RemappingCategoryAnchor.h"
#include "KeyRemappingMenu.generated.h"

/**
 * This widget is the menu used for remapping keys. 
 * It spawns in IndividualKeyMaps as needed. 
 */

UCLASS()
class ADVANCEDRTSCAMERA_API UKeyRemappingMenu : public UOptionsMenuMaster
{
	GENERATED_BODY()

	friend class UIndividualKeyMap;
	friend class UPauseMenu;

/************************************************
* ATTRIBUTES
************************************************/
protected:
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/
	/** This is a popup widget used to alert the player they are reverting/reseting all keybindings to defualt*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UConfirmResetMapping* ConfirmResetMenu;
	/** This is the anchor (in the scrollbox) for the section heads and selectors */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UVerticalBox* InputsBox;

	/************************************************
	* REFERENCE ATTRIBUTES
	************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UGameInstance* GameInst;

	/************************************************
	* VARIABLE ATTRIBUTES
	************************************************/
	/** Widgets of the key bindings */
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TArray<UIndividualKeyMap*> InputSelectors;
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TArray<URemappingCategoryAnchor*> CategoryAnchors;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Components|Settings")
	TArray<FText> MapCategories;

	UPROPERTY(BlueprintReadWrite, Category = "Targets")
	UPlayerMappableInputConfig* MappableBindings;


/************************************************
* METHODS
************************************************/
protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void InitMenu(UPlayerMappableInputConfig* PMI);
	/* Implemented in the BP child */
	UFUNCTION(BlueprintImplementableEvent, Category = "SetUp")
	void SpawnCategoryPanels();
	/* Implemented in the BP child */
	UFUNCTION(BlueprintImplementableEvent, Category = "SetUp")
	void SpawnSelectors();
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void DeleteUnusedCategories();
	/* Currently not used but here for backup and if needed */
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void RespawnSelectors();

	void OnCancel_Implementation() override;
	void OnAccept_Implementation() override;
	void OnDefault_Implementation() override;
	void ForceClose_Implementation() override;
	void ResetChanges_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Event")
	void OnUpdateValues();
	UFUNCTION(BlueprintCallable, Category = "Event")
	bool CheckValidity(FInputChord NewKey);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Checks")
	FORCEINLINE bool CheckIsValidKeyboardAndMouseInput(FEnhancedActionKeyMapping TargetMapping) const
	{
		return ((TargetMapping.Key.IsMouseButton() || TargetMapping.Key.IsAxis3D() || UKismetInputLibrary::Key_IsKeyboardKey(TargetMapping.Key))
			&& (TargetMapping.IsPlayerMappable())
			&& (!TargetMapping.Key.IsGesture()));
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Checks")
	FORCEINLINE bool CheckIsValidControllerInput(FEnhancedActionKeyMapping TargetMapping) const
	{
		return (((!(TargetMapping.Key.IsMouseButton() || TargetMapping.Key.IsAxis3D() || UKismetInputLibrary::Key_IsKeyboardKey(TargetMapping.Key)))
				|| (TargetMapping.Key.IsGamepadKey()))
			&& (TargetMapping.IsPlayerMappable())
			&& (!TargetMapping.Key.IsGesture()));
	}
};

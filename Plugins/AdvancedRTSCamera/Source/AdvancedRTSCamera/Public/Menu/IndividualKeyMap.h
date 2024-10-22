// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/InputKeySelector.h"
#include "Components/Button.h"
#include "InputCoreTypes.h"
#include "InputAction.h"
#include "PlayerMappableInputConfig.h"
#include "Systems/SettingsSaveMaster.h"

//#include "InputMappingContext.h" // not used

#include "IndividualKeyMap.generated.h"

/**
 * This widget controls the rebinding of both primary and secondary inputs
 * This widget is spawned by KeyMapping Menu.
 */
UCLASS()
class ADVANCEDRTSCAMERA_API UIndividualKeyMap : public UUserWidget
{
	GENERATED_BODY()

	friend class UKeyRemappingMenu;

/************************************************
* ATTRIBUTES
************************************************/
protected:
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UTextBlock* ActionName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UInputKeySelector* KeySelector;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* ResetButton;

	/************************************************
	* REFERENCE ATTRIBUTES
	************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UKeyRemappingMenu* ParentWidget;
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UGameInstance* GameInst;

	/************************************************
	* VARIABLE ATTRIBUTES
	************************************************/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SetUp")
	bool bUseIcons{ false };

	/** The  input action KEY; stored on setup and edited on rebind */
	UPROPERTY(BlueprintReadWrite, Category = "Keys|Current")
	FKey CurrentKey;
	/** The  input action KEY; stored on setup and edited on rebind */
	UPROPERTY(BlueprintReadWrite, Category = "Keys|New")
	FKey NewKey;
	
	/** The target Input Action */
	//UPROPERTY(BlueprintReadWrite, Category = "Targets")
	//UInputAction* TargetInput; // TODO we don't use this...
	UPROPERTY(BlueprintReadWrite, Category = "Targets")
	UPlayerMappableInputConfig* MappableBindings;
	UPROPERTY(BlueprintReadWrite, Category = "Targets")
	USettingsSaveMaster* KeyBinds;

	/** The primary KEY MAP; stored on setup and edited on rebind */
	UPROPERTY(BlueprintReadWrite, Category = "Maps")
	FEnhancedActionKeyMapping KeyMapping;

	UPROPERTY(BlueprintReadWrite, Category = "States")
	bool bPendingChanges{ false };
	UPROPERTY(BlueprintReadWrite, Category = "States")
	bool bRemapping{ false };

/************************************************
* METHODS
************************************************/
protected:
	bool Initialize() override;

	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void InitKeySelector(UKeyRemappingMenu* Parent, UPlayerMappableInputConfig* PMI, FEnhancedActionKeyMapping Map);
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void SetSelectorVariables(UKeyRemappingMenu* Parent, UPlayerMappableInputConfig* PMI, FEnhancedActionKeyMapping Map);
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void SetActionName();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SetUp")
	bool SetSelectorKeyValue();
	bool SetSelectorKeyValue_Implementation();
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void SetResetButton();
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void ReInit();


	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnKeyChanged();
	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnKeySelected(FInputChord SelectedKey);
	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnResetKey(); 
	UFUNCTION(BlueprintCallable, Category = "Events")
	void AcceptChanges();
	UFUNCTION(BlueprintCallable, Category = "Events")
	void CancelChanges();
	UFUNCTION(BlueprintCallable, Category = "Events")
	void ResetKey();

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void SetKeyIcon(FKey Key);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void HideKeyIcon(bool State);
};

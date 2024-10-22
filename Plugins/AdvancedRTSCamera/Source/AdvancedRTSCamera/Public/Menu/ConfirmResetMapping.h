// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ConfirmResetMapping.generated.h"

/**
 * A pop-up widget used to let users know they have remapped keys that will be overriden
 */
UCLASS()
class ADVANCEDRTSCAMERA_API UConfirmResetMapping : public UUserWidget
{
	GENERATED_BODY()

	friend class UKeyRemappingMenu;

protected:
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/
	/** Button for returning to settings menu / canceling window change */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* PrecedeButton;
	/** Button for confirming that the user indeed wants to cancel submitting changes */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* CancelButton;

	/************************************************
	* REFERENCE ATTRIBUTES
	************************************************/
	/** Reference to the visual scalability settings menu */
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UOptionsMenuMaster* TargetMenu;

/************************************************
* METHODS
************************************************/
protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void SetSettingScreen(UOptionsMenuMaster* OwningWidget);

	UFUNCTION(BlueprintCallable, Category = "Event")
	void OnPrecede();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void OnCancel();
};

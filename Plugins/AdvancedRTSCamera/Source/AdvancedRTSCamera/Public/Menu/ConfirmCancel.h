// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ConfirmCancel.generated.h"

/**
 * A pop-up widget used to let users know they have changed but not applied settings.
 */
UCLASS()
class ADVANCEDRTSCAMERA_API UConfirmCancel : public UUserWidget
{
	GENERATED_BODY()

	friend class UOptionsMenuMaster;
	friend class UVisualSettingsMenu;
	friend class UKeyRemappingMenu;
	friend class UAudioSettingMenu;
	friend class UGameplaySettingsMenu;

protected:
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/
	/** Button for returning to settings menu / canceling window change */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* GoBackButton;
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
	bool Initialize() override;

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void SetSettingScreen(UOptionsMenuMaster* OwningWidget);

	UFUNCTION(BlueprintCallable, Category = "Event")
	void OnReturn();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void OnForceClose();

};

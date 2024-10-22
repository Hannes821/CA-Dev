// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "ConfirmCancel.h"
#include "ConfirmResetMapping.h"
#include "OptionsMenuMaster.generated.h"

/**
 * This class is the widget master class for all setting menus (scalability, keymapping, audio).
 * It should never be used as a direct set of code for a WBP - unless the BP is heavily edited.
 */
UCLASS(Abstract)
class ADVANCEDRTSCAMERA_API UOptionsMenuMaster : public UUserWidget
{
	GENERATED_BODY()

	friend class UConfirmCancel;
	friend class UConfirmResetMapping;
	friend class UGameplaySettingsMenu;

protected:
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/
	/** The anchor that holds the scrollbox and the buttons. 
	This box is disabled and enabled at certain points. */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UVerticalBox* SettingsVerticalAnchor;
	
	/** The scroll box for scrolling through the options. */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UScrollBox* SettingScroll;

	/** Button for restoring defaults/autodetecting best settings */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* DefaultButton;
	/** Button for accepting new settings, starts collapsed */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* AcceptButton;
	/** Button for cancelling for settings/closeding menu */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UButton* CancelButton;
	/** Pop-up summoned if user tries to close while changes are pending/detected */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UConfirmCancel* ConfirmCancelMenu;

	/************************************************
	* VARIABLE ATTRIBUTES
	************************************************/
	/** Has this menu detected a change? This effects the AcceptButton and CancelButton */
	UPROPERTY(BlueprintReadWrite, Category = "Status")
	bool bChangeDetected{ false };

/************************************************
* METHODS
************************************************/
protected:
	virtual void NativeConstruct() override;


	/* Events that happen when the cancel button is pressed */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Event")
	void OnCancel();
	virtual void OnCancel_Implementation();
	/* Event that happen then the accept button is pressed */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Event")
	void OnAccept();
	virtual void OnAccept_Implementation();
	/* Events that happen when the default button is selected */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Event")
	void OnDefault();
	virtual void OnDefault_Implementation();
	/* Close the menu even if there are pending changes without saving them */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Event")
	void ForceClose();
	virtual void ForceClose_Implementation();
	/* Allows the user to continue editing settings after an aborted attempt to cancel with pending changes */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Event")
	void ResumeEditing();
	virtual void ResumeEditing_Implementation();
	/* Undoes pending changes */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Event")
	void ResetChanges();
	virtual void ResetChanges_Implementation();
};

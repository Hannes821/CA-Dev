// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Systems/SettingsSaveMaster.h"
#include "InputSaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInputSaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface handles communication to/from the input settings widget and the save system
 */
class ADVANCEDRTSCAMERA_API IInputSaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "General")
	USettingsSaveMaster* GetSavedSettings();
	
	/* Used to call load bindings from other classes than the class that inherits from this interface*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Keybinding|Saves")
	void TriggerLoadBindings();
	/* Used to call save bindings from other classes than the class that inherits from this interface*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Keybinding|Saves")
	void TriggerSaveBindings();
	/** Use to update existing custom key bind */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Keybinding|Saves")
	void UpdateKeyBindings(const FName& Name, const FKey& Key, int Index);
	/* Used to add new custom key bind */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Keybinding|Saves")
	void AddKeyBinding(const FName& Name, const FKey& Key);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Keybinding|Saves")
	void RemoveBindingByIndex(int Index);
};

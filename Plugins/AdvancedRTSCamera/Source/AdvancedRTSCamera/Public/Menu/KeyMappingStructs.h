// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "KeyMappingStructs.generated.h"

/**
 * Structs used in key mapping/remapping of bound inputs. 
 */


/** This is the struct used to save and parse custom key bindings. */
USTRUCT(BlueprintType)
struct FKeyBindingsSave
{
	GENERATED_BODY()

	/** A list of fkeys that have been overriden. */
	UPROPERTY(BlueprintReadOnly, Category = "Keybindings")
	TArray<FKey> KeyOverrides;

	/** A list of overriden mapping names */
	UPROPERTY(BlueprintReadOnly, Category = "Keybindings")
	TArray<FName> MappingNames;
};


// DEPRICATED

/** The structure for a single specific action that can be mapped/remapped. 
This struct should typically not be used on its own, but as an array as a part of FMappingContext.
It directly references an InputAction object. */
USTRUCT(BlueprintType)
struct FInputContext
{
	GENERATED_BODY()

	/** The name that is displayed in the menu and the name of the remapping.
	The remapping name is used to ensure the right action is changed.
	This can come up when there are more than one inputs to an InputAction. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Keybindings")
	FName TargetAction;

	/** The actual InputAction file that contains the target action. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Keybindings")
	UInputAction* InputAction;
};

/** This is a struct that contains all the remappable actions, what InputMappingConext they can be found it, a backup.
As this struct edits the original InputMappingConext it is strongly recommended that you use a backup context as well.
The backup can be used to restore the original context. 
There can be multiple of these used, based on different InputMappingConext being employed 
and/or based on different categories. */
USTRUCT(BlueprintType)
struct FMappingContext
{
	GENERATED_BODY()
	
	/** A list of all the InputActions (and their names) that this MappingContext can remap.
	These should be used to determine how many FMappingContexts are needed - split based on category/topic for example. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Keybindings")
	TArray<FInputContext> InputDetails;

	/** The InputMappingContext that contains the mapping and InputAction.
	THIS GETS EDITED AT RUN TIME! */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Keybindings")
	UInputMappingContext* MappingContext;

	/** A backup of the original MappingContext struct member. Used to restore the original InputMappingContext.
	Do NOT use the same object, make a COPY of the original - BEFORE USING! */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Keybindings")
	UInputMappingContext* BackupContext;
};

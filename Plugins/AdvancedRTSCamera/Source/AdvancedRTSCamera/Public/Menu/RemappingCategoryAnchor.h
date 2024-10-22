// Copywritten by Two Neurons Studio, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "RemappingCategoryAnchor.generated.h"

/**
 * This is a widget class used to create categories for input remapping
 * It has a title for the type of remapping (e.g., combat, movement, etc).
 * And it has an anchor for the relevent remappings
 */
UCLASS()
class ADVANCEDRTSCAMERA_API URemappingCategoryAnchor : public UUserWidget
{
	GENERATED_BODY()

/************************************************
* ATTRIBUTES
************************************************/
protected:
	/************************************************
	* BOUND ATTRIBUTES
	************************************************/	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UTextBlock* SectionTitle;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
	class UVerticalBox* AnchorBox;

	/************************************************
	* VARIABLE ATTRIBUTES
	************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Details")
	FText Title;

/************************************************
* METHODS
************************************************/
public:
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void SetSectionTitle(const FText& TitleIn);
	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void TriggerAddChild(UUserWidget* Widget);
	UFUNCTION(BlueprintCallable, Category = "Reset")
	void TriggerClearChildren();


	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Getters")
	FORCEINLINE FText GetTitle() const { return Title; }
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Getters")
	FORCEINLINE bool TriggerHasChildren() const { return AnchorBox->HasAnyChildren(); }
};

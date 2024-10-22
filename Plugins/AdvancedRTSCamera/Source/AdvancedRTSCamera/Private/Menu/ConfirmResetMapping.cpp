// Copywritten by Two Neurons Studio, LLC. All rights reserved.


#include "Menu/ConfirmResetMapping.h"
#include "Menu/OptionsMenuMaster.h"


void UConfirmResetMapping::NativeConstruct()
{
	Super::NativeConstruct();

	PrecedeButton->OnClicked.Clear();
	PrecedeButton->OnClicked.AddDynamic(this, &UConfirmResetMapping::OnPrecede);
	CancelButton->OnClicked.Clear();
	CancelButton->OnClicked.AddDynamic(this, &UConfirmResetMapping::OnCancel);
}

void UConfirmResetMapping::SetSettingScreen(UOptionsMenuMaster* OwningWidget)
{
	TargetMenu = OwningWidget;
}

void UConfirmResetMapping::OnPrecede()
{
	TargetMenu->ResetChanges();
}

void UConfirmResetMapping::OnCancel()
{
	TargetMenu->ResumeEditing();
	SetVisibility(ESlateVisibility::Hidden);
}

// Copywritten by Two Neurons Studio, LLC. All rights reserved.


#include "Menu/ConfirmCancel.h"
#include "Menu/OptionsMenuMaster.h"


bool UConfirmCancel::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	return true;
}

void UConfirmCancel::NativeConstruct()
{
	Super::NativeConstruct();

	GoBackButton->OnClicked.Clear();
	GoBackButton->OnClicked.AddDynamic(this, &UConfirmCancel::OnReturn);
	CancelButton->OnClicked.Clear();
	CancelButton->OnClicked.AddDynamic(this, &UConfirmCancel::OnForceClose);
}

void UConfirmCancel::SetSettingScreen(UOptionsMenuMaster* OwningWidget)
{
	TargetMenu = OwningWidget;
}

void UConfirmCancel::OnForceClose()
{
	TargetMenu->ForceClose();
}

void UConfirmCancel::OnReturn()
{
	TargetMenu->ResumeEditing();
}

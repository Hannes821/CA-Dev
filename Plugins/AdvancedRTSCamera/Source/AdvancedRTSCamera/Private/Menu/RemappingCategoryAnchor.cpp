// Copywritten by Two Neurons Studio, LLC. All rights reserved.


#include "Menu/RemappingCategoryAnchor.h"

void URemappingCategoryAnchor::SetSectionTitle(const FText& TitleIn)
{
	Title = TitleIn;
	SectionTitle->SetText(Title);
}

void URemappingCategoryAnchor::TriggerAddChild(UUserWidget* Widget)
{
	AnchorBox->AddChildToVerticalBox(Widget);
}

void URemappingCategoryAnchor::TriggerClearChildren()
{
	AnchorBox->ClearChildren();
}

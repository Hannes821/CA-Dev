// Code written by Zachary Parker and Hannes Ulbricht for Feldfeste 


#include "Core/Widgets/ToolsMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Player/RTSPlayerControllerBase.h"

UToolsMenu::UToolsMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool UToolsMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	//if (!ensure(AimsButton != nullptr)) return false;
	////AimsButton->OnClicked.AddDynamic(this, &UToolsMenu::DisplayAimsMenu);

	if (!ensure(ConstructionButton != nullptr)) return false;
	ConstructionButton->OnClicked.AddDynamic(this, &UToolsMenu::DisplayConstructionMenu);

	//if (!ensure(OverviewButton != nullptr)) return false;
	//OverviewButton->OnClicked.AddDynamic(this, &UToolsMenu::DisplayOverviewMenu);

	//if (!ensure(ChatButton != nullptr)) return false;
	//ChatButton->OnClicked.AddDynamic(this, &UToolsMenu::DisplayChat);

	//if (!ensure(DiplomancyButton != nullptr)) return false;
	//DiplomancyButton->OnClicked.AddDynamic(this, &UToolsMenu::DisplayDiplomacyMenu);

	//if (!ensure(OptionsButton != nullptr)) return false;
	//OptionsButton->OnClicked.AddDynamic(this, &UToolsMenu::DisplayOptionsMenu);

	if (!ensure(HouseButton != nullptr)) return false;
	HouseButton->OnClicked.AddDynamic(this, &UToolsMenu::BuildHouse);

	if (!ensure(GrannaryButton != nullptr)) return false;
	GrannaryButton->OnClicked.AddDynamic(this, &UToolsMenu::BuildGranary);

	return true;
}

void UToolsMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// default the sub menus to hidden
	if (!ensure(ConstructionsMenuSizeBox != nullptr)) return;
	ConstructionsMenuSizeBox->SetVisibility(ESlateVisibility::Hidden);
}

void UToolsMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

/************************************************
* DISPLAY MENUS
************************************************/
//void UToolsMenu::DisplayAimsMenu()
//{
//}

void UToolsMenu::DisplayConstructionMenu()
{
	if (bIsConstructionMenuOpen == false)
	{
		// If the menu is closed, open it.
		ShowConstructionMenu(ESlateVisibility::SelfHitTestInvisible);
		bIsConstructionMenuOpen = true;
	}
	else if (bIsConstructionMenuOpen == true)
	{
		// if the menu is open, close it.
		ShowConstructionMenu(ESlateVisibility::Hidden);
		bIsConstructionMenuOpen = false;
	}
	// DO NOT ADD RETURNS TO THESE FUNCTIONS! (just found a cool engine bug)
}

void UToolsMenu::ShowConstructionMenu(ESlateVisibility InVisibility)
{
	if (!ensure(ConstructionsMenuSizeBox != nullptr)) return;
	ConstructionsMenuSizeBox->SetVisibility(InVisibility);
}

//void UToolsMenu::DisplayOverviewMenu()
//{
//}

//void UToolsMenu::DisplayChat()
//{
//}

//void UToolsMenu::DisplayDiplomacyMenu()
//{
//}

//void UToolsMenu::DisplayOptionsMenu()
//{
//}

/************************************************
* CONSTRUCTION OPTIONS
************************************************/
void UToolsMenu::ConstructPreview(FName InRowName)
{
	APlayerController* PlayerControllerLocal = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerControllerLocal->GetClass()->ImplementsInterface(UPlayerControllerInterface::StaticClass()))
	{
		IPlayerControllerInterface::Execute_GetBuildingManager(PlayerControllerLocal)->ROC_SetPreview(InRowName);
		IPlayerControllerInterface::Execute_TriggerSetActionMode(PlayerControllerLocal, EPrimaryActionModes::Construction);
	}
	/*
	Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetClass()->ImplementsInterface(UPlayerControllerInterface::StaticClass());
	IPlayerControllerInterface::Execute_GetBuildingManager(PlayerControllerLocal)->ROC_SpawnPreview(InRowName); */
}

void UToolsMenu::BuildHouse()
{
	ConstructPreview(FName("House"));
	
	// hide menu
	if (bIsConstructionMenuOpen == true)
	{
		// if the menu is open, close it.
		ShowConstructionMenu(ESlateVisibility::Hidden);
		bIsConstructionMenuOpen = false;
	}
}

void UToolsMenu::BuildGranary()
{
	ConstructPreview(FName("Granary"));

	// hide menu
	if (bIsConstructionMenuOpen == true)
	{
		// if the menu is open, close it.
		ShowConstructionMenu(ESlateVisibility::Hidden);
		bIsConstructionMenuOpen = false;
	}
}

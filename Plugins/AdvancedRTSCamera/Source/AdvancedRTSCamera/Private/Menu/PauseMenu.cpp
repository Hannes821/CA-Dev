// Copyright Two Neurons Studio, LLC. Year of Publication 2024.


#include "Menu/PauseMenu.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/PauseMenuInterface.h"

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// bind mouse click events (after clearing) - and it is recommended to make the click mode precise in WBP child class
	ResumeButton->OnClicked.Clear();
	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::OnClickResume);
	GraphicsButton->OnClicked.Clear();
	GraphicsButton->OnClicked.AddDynamic(this, &UPauseMenu::OnClickGraphics);
	BindingsButton->OnClicked.Clear();
	BindingsButton->OnClicked.AddDynamic(this, &UPauseMenu::OnClickBindings);
	GameplayButton->OnClicked.Clear();
	GameplayButton->OnClicked.AddDynamic(this, &UPauseMenu::OnClickGameplaySettings);
	AudioSettingsButton->OnClicked.Clear();
	AudioSettingsButton->OnClicked.AddDynamic(this, &UPauseMenu::OnClickAudio);
	QuitGameButton->OnClicked.Clear();
	QuitGameButton->OnClicked.AddDynamic(this, &UPauseMenu::OnClickQuit);

	MenuSwitcher->SetActiveWidget(DefaultBlank);
}


void UPauseMenu::InitSettings(UPlayerMappableInputConfig* PMI, TArray<FMovementSettings> GameplaySettingsIn, TArray<float> GameplayDefaultsIn)
{
	KeyRemapping->InitMenu(PMI);

	if (GameplaySettingsIn.Num() > 0)
		GameplaySettings->InitSettings(GameplaySettingsIn, GameplayDefaultsIn);
}

void UPauseMenu::OnClickResume()
{
	APlayerController* PlayerControllerLocal = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	check(PlayerControllerLocal);
	APawn* PlayerPawnLocal = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	check(PlayerPawnLocal); // if this fails holy god something is wrong...


	// update bindings on either controller OR pawn
	if (PlayerControllerLocal->GetClass()->ImplementsInterface(UPauseMenuInterface::StaticClass()))// Determine if Controller implements InputMapInterface
	{
		IPauseMenuInterface::Execute_ResumeGame(PlayerControllerLocal);
	}
	else if (PlayerPawnLocal->GetClass()->ImplementsInterface(UPauseMenuInterface::StaticClass()))// Determine if Player Pawn implements InputMapInterface
	{
		IPauseMenuInterface::Execute_ResumeGame(PlayerPawnLocal);
	}
	else
	{
		// put an error log here that neither the controller or pawn use the interface and cannot be updated
	}
}

void UPauseMenu::OnClickGraphics()
{
	MenuSwitcher->SetActiveWidget(VisualSettings);
}

void UPauseMenu::OnClickBindings()
{
	MenuSwitcher->SetActiveWidget(KeyRemapping);
}

void UPauseMenu::OnClickAudio()
{
	MenuSwitcher->SetActiveWidget(AudioSettings);
}

void UPauseMenu::OnClickGameplaySettings()
{
	MenuSwitcher->SetActiveWidget(GameplaySettings);
}

void UPauseMenu::OnClickQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}

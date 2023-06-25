// Code written by Zachary Parker for Feldfeste 


#include "Core/Settings/RTSGameInstance.h"
#include "Engine/Engine.h"

/* constructor*/
URTSGameInstance::URTSGameInstance(const FObjectInitializer& ObjectInitializer)
{

}

/* init */
void URTSGameInstance::Init()
{

}

void URTSGameInstance::Host()
{
	/* just showing a screen message to make sure it works*/
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return; // make sure engine is not null

	Engine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("Hosting"));

	/* Travel from lobby map to main map*/
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return; // make sure world is not null

	World->ServerTravel("/Game/Maps/TestMap?listen"); // Travel to Test RTS MAP as a listen server, will refine further later
}

/* Join game - for debugging USE YOUR LOCAL IP!*/
void URTSGameInstance::Join(const FString& Address) 
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return; // make sure engine is not null

	Engine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	/* Travel from lobby to hosted map*/
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return; // make sure controller is not null

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

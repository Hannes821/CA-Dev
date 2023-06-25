// Code written by Zachary Parker for Feldfeste 


#include "Core/Widgets/MainUIBase.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/Layout/Anchors.h"
#include "UObject/ConstructorHelpers.h"

/*UMainUIBase::UMainUIBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuClassFinder(TEXT("/Game/Core/Widgets/MainComponents/WBP_GameTime"));
	ClockClass = MenuClassFinder.Class;
}*/

void UMainUIBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainUIBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

/*void UMainUIBase::SetClockWidget(UGameClockWidget* ChildWidget)
{
	if (ChildWidget != nullptr)
	{
		GameClock = ChildWidget;
	}
}*/

bool UMainUIBase::SetGameDateTime_Implementation(FDateTime TimeStruct)
{
	if (!ensure(GameClock != nullptr)) return false;

	DateTimeStruct = TimeStruct;
	PassDateTimeStruct(DateTimeStruct, GameClock);
	GameClock->CheckButtons();
	return true;
}

bool UMainUIBase::SetGameDay_Implementation(int GameDay)
{
	if (!ensure(GameClock != nullptr)) return false;


	Day = GameDay;
	PassDayOfWeek(Day, GameClock);
	GameClock->CheckButtons();
	return true;
}

void UMainUIBase::PassDateTimeStruct(FDateTime TimeStruct, UGameClockWidget* ClockIn)
{
	if (ClockIn != nullptr)
	{
		ClockIn->GetClass()->ImplementsInterface(UGameTimeInterface::StaticClass());
		IGameTimeInterface::Execute_SetGameDateTime(GameClock, TimeStruct);
	}
}

void UMainUIBase::PassDayOfWeek(int GameDay, UGameClockWidget* ClockIn)
{
	if (ClockIn != nullptr)
	{
		ClockIn->GetClass()->ImplementsInterface(UGameTimeInterface::StaticClass());
		IGameTimeInterface::Execute_SetGameDay(GameClock, GameDay);
	}
}

void UMainUIBase::PassGameSpeed(float GameSpeed, UGameClockWidget* ClockIn)
{
	if (ClockIn != nullptr)
	{
		ClockIn->GetClass()->ImplementsInterface(UGameTimeInterface::StaticClass());
		IGameTimeInterface::Execute_SetGameSpeedInt(GameClock, GameSpeed);
	}
}

bool UMainUIBase::SetGameSpeedInt_Implementation(float GameSpeed)
{
	if (!ensure(GameClock != nullptr)) return false;

	PassGameSpeed(GameSpeed, GameClock);
	return true;
}

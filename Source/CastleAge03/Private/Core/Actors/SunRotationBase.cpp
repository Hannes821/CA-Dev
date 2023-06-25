// Code written by Zachary Parker for Feldfeste 


#include "Core/Actors/SunRotationBase.h"
#include "GameFramework/Actor.h"

// Sets default values
ASunRotationBase::ASunRotationBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Replicated variables
void ASunRotationBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ASunRotationBase, SunRotation);
	DOREPLIFETIME_CONDITION_NOTIFY(ASunRotationBase, SunRotation, COND_None, REPNOTIFY_Always);

}


// Called when the game starts or when spawned
void ASunRotationBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASunRotationBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (SunRef != nullptr)
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			// MC_RotateSun();
			SunRotation = CalculateSunRotation(ConvertTime(DateTimeStruct));
			if (GetNetMode() != NM_DedicatedServer)
			{
				OnRep_SunRotation();
			}
		}
	}
	else
	{
		// if there is no sun then this class is not neeeded, destroy it.
		Destroy();
	}
}

float ASunRotationBase::ConvertTime(FDateTime TimeStruct)
{
	return ((float)TimeStruct.GetHour()) + (((float)TimeStruct.GetMinute()) / 60.0f) + (((float)TimeStruct.GetSecond()) / 3600.0f);;
}

FRotator ASunRotationBase::CalculateSunRotation(float GameTime)
{
	// feel free to remove the first argument (*-1).
	return FRotator(((((fmod((GameTime * -1.0f), 24.0f)) / 24) * 360.0f) + 90.0f), 0.0f, 0.0f);
}

void ASunRotationBase::MC_RotateSun_Implementation()
{
	SunRef->SetActorRotation(CalculateSunRotation(ConvertTime(DateTimeStruct)));
}

void ASunRotationBase::OnRep_SunRotation()
{
	SunRef->SetActorRotation(SunRotation);
}

bool ASunRotationBase::SetGameDateTime_Implementation(FDateTime TimeStruct)
{
	DateTimeStruct = TimeStruct;
	return true;
}


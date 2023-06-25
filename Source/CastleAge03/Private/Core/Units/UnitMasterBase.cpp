// Code written by Zachary Parker for Feldfeste 


#include "Core/Units/UnitMasterBase.h"

// Sets default values
AUnitMasterBase::AUnitMasterBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnitMasterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnitMasterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUnitMasterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


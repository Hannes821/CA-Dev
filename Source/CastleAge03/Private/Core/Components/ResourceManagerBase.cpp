// Code written by Zachary Parker for Feldfeste 


#include "Core/Components/ResourceManagerBase.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UResourceManagerBase::UResourceManagerBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// TODO replace with real setup later
	for (size_t i = 0; i < 30; i++)
	{
		ROS_SetMaxResourceAmount(static_cast<EResources>(i), 100);
	}

	// ...
}

// Replicated variables
void UResourceManagerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UResourceManagerBase, PlayerResources);
	DOREPLIFETIME(UResourceManagerBase, Remainder);
	DOREPLIFETIME(UResourceManagerBase, bHasRemainder);
}


// Called when the game starts
void UResourceManagerBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UResourceManagerBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UResourceManagerBase::ROS_SetResourceAmount_Implementation(EResources Value, float Amount)
{
	float NewAmountLocal{ 0.0f };
	Remainder = 0.0f;
	bHasRemainder = false;

	switch (Value)
	{
	case Clay:
		NewAmountLocal = PlayerResources.Clay.CurrentValue += Amount;

		PlayerResources.Clay.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Clay.MaxValue);

		if (NewAmountLocal > PlayerResources.Clay.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Clay.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Coal:
		NewAmountLocal = PlayerResources.Coal.CurrentValue += Amount;

		PlayerResources.Coal.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Coal.MaxValue);

		if (NewAmountLocal > PlayerResources.Coal.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Coal.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
		// PlayerResources.Coal.CurrentValue += Amount;
	case Flax:
		NewAmountLocal = PlayerResources.Flax.CurrentValue += Amount;

		PlayerResources.Flax.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Flax.MaxValue);

		if (NewAmountLocal > PlayerResources.Flax.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Flax.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
		// PlayerResources.Flax.CurrentValue = (PlayerResources.Flax.CurrentValue += Amount > PlayerResources.Flax.MaxValue) ? PlayerResources.Flax.MaxValue : PlayerResources.Flax.CurrentValue += Amount;
		// PlayerResources.Flax.CurrentValue += Amount;
	case IronOrePowder:
		NewAmountLocal = PlayerResources.IronOrePowder.CurrentValue += Amount;

		PlayerResources.IronOrePowder.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.IronOrePowder.MaxValue);

		if (NewAmountLocal > PlayerResources.IronOrePowder.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.IronOrePowder.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Leather:
		NewAmountLocal = PlayerResources.Leather.CurrentValue += Amount;

		PlayerResources.Leather.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Leather.MaxValue);

		if (NewAmountLocal > PlayerResources.Leather.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Leather.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Brick:
		NewAmountLocal = PlayerResources.Brick.CurrentValue += Amount;

		PlayerResources.Brick.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Brick.MaxValue);

		if (NewAmountLocal > PlayerResources.Brick.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Brick.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Tar:
		NewAmountLocal = PlayerResources.Tar.CurrentValue += Amount;

		PlayerResources.Tar.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Tar.MaxValue);

		if (NewAmountLocal > PlayerResources.Tar.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Tar.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case WoodBeam:
		NewAmountLocal = PlayerResources.WoodBeam.CurrentValue += Amount;

		PlayerResources.WoodBeam.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.WoodBeam.MaxValue);

		if (NewAmountLocal > PlayerResources.WoodBeam.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.WoodBeam.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Wool:
		NewAmountLocal = PlayerResources.Wool.CurrentValue += Amount;

		PlayerResources.Wool.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Wool.MaxValue);

		if (NewAmountLocal > PlayerResources.Wool.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Wool.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Cloth:
		NewAmountLocal = PlayerResources.Cloth.CurrentValue += Amount;
		
		PlayerResources.Cloth.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Cloth.MaxValue);

		if (NewAmountLocal > PlayerResources.Cloth.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Cloth.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case OrePowder:
		NewAmountLocal = PlayerResources.OrePowder.CurrentValue += Amount;

		PlayerResources.OrePowder.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.OrePowder.MaxValue);

		if (NewAmountLocal > PlayerResources.OrePowder.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.OrePowder.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Saddle:
		NewAmountLocal = PlayerResources.Saddle.CurrentValue += Amount;
		
		PlayerResources.Saddle.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Saddle.MaxValue);

		if (NewAmountLocal > PlayerResources.Saddle.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Saddle.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case WoodPlank:
		NewAmountLocal = PlayerResources.WoodPlank.CurrentValue += Amount;

		PlayerResources.WoodPlank.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.WoodPlank.MaxValue);

		if (NewAmountLocal > PlayerResources.WoodPlank.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.WoodPlank.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Bow:
		NewAmountLocal = PlayerResources.Bow.CurrentValue += Amount;
		
		PlayerResources.Bow.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Bow.MaxValue);

		if (NewAmountLocal > PlayerResources.Bow.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Bow.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Chainmail:
		NewAmountLocal = PlayerResources.Chainmail.CurrentValue += Amount;

		PlayerResources.Chainmail.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Chainmail.MaxValue);

		if (NewAmountLocal > PlayerResources.Chainmail.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Chainmail.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Crossbow:
		NewAmountLocal = PlayerResources.Crossbow.CurrentValue += Amount;

		PlayerResources.Crossbow.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Crossbow.MaxValue);

		if (NewAmountLocal > PlayerResources.Crossbow.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Crossbow.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Gambeson:
		NewAmountLocal = PlayerResources.Gambeson.CurrentValue += Amount;

		PlayerResources.Gambeson.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Gambeson.MaxValue);

		if (NewAmountLocal > PlayerResources.Gambeson.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Gambeson.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Javelins:
		NewAmountLocal = PlayerResources.Javelins.CurrentValue += Amount;

		PlayerResources.Javelins.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Javelins.MaxValue);

		if (NewAmountLocal > PlayerResources.Javelins.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Javelins.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Spear:
		NewAmountLocal = PlayerResources.Spear.CurrentValue += Amount;

		PlayerResources.Spear.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Spear.MaxValue);

		if (NewAmountLocal > PlayerResources.Spear.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Spear.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Sword:
		NewAmountLocal = PlayerResources.Sword.CurrentValue += Amount;

		PlayerResources.Sword.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Sword.MaxValue);

		if (NewAmountLocal > PlayerResources.Sword.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Sword.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Shield:
		NewAmountLocal = PlayerResources.Shield.CurrentValue += Amount;

		PlayerResources.Shield.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Shield.MaxValue);

		if (NewAmountLocal > PlayerResources.Shield.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Shield.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Cheese:
		NewAmountLocal = PlayerResources.Cheese.CurrentValue += Amount;

		PlayerResources.Cheese.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Cheese.MaxValue);

		if (NewAmountLocal > PlayerResources.Cheese.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Cheese.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Fish:
		NewAmountLocal = PlayerResources.Fish.CurrentValue += Amount;

		PlayerResources.Fish.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Fish.MaxValue);

		if (NewAmountLocal > PlayerResources.Fish.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Fish.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Fruits:
		NewAmountLocal = PlayerResources.Fruits.CurrentValue += Amount;

		PlayerResources.Fruits.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Fruits.MaxValue);

		if (NewAmountLocal > PlayerResources.Fruits.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Fruits.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Grain:
		NewAmountLocal = PlayerResources.Grain.CurrentValue += Amount;
		
		PlayerResources.Grain.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Grain.MaxValue);

		if (NewAmountLocal > PlayerResources.Grain.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Grain.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Hop:
		NewAmountLocal = PlayerResources.Hop.CurrentValue += Amount;

		PlayerResources.Hop.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Hop.MaxValue);

		if (NewAmountLocal > PlayerResources.Hop.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Hop.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Sausages:
		NewAmountLocal = PlayerResources.Sausages.CurrentValue += Amount;

		PlayerResources.Sausages.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Sausages.MaxValue);

		if (NewAmountLocal > PlayerResources.Sausages.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Sausages.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Vegetables:
		NewAmountLocal = PlayerResources.Vegetables.CurrentValue += Amount;

		PlayerResources.Vegetables.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Vegetables.MaxValue);

		if (NewAmountLocal > PlayerResources.Vegetables.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Vegetables.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Beer:
		NewAmountLocal = PlayerResources.Beer.CurrentValue += Amount;

		PlayerResources.Beer.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Beer.MaxValue);

		if (NewAmountLocal > PlayerResources.Beer.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Beer.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Flour:
		NewAmountLocal = PlayerResources.Flour.CurrentValue += Amount;

		PlayerResources.Flour.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Flour.MaxValue);

		if (NewAmountLocal > PlayerResources.Flour.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Flour.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	case Bread:
		NewAmountLocal = PlayerResources.Bread.CurrentValue += Amount;

		PlayerResources.Bread.CurrentValue = FMath::Clamp((NewAmountLocal), 0.0f, PlayerResources.Bread.MaxValue);

		if (NewAmountLocal > PlayerResources.Bread.MaxValue)
		{
			Remainder = NewAmountLocal - PlayerResources.Bread.MaxValue;
			bHasRemainder = true;
		}
		else
		{
			Remainder = 0.0f;
			bHasRemainder = false;
		}
		break;
	default:
		bHasRemainder = false;
		Remainder = 0.0f;
	}
	return;
}

void UResourceManagerBase::ROS_SetMaxResourceAmount_Implementation(EResources Value, float Amount)
{
	switch (Value)
	{
	case AnimalHide:
		PlayerResources.AnimalHide.MaxValue += Amount;
		return;
	case BattleAxe:
		PlayerResources.BattleAxe.MaxValue += Amount;
		return;
	case Beer:
		PlayerResources.Beer.MaxValue += Amount;
		return;
	case Berries:
		PlayerResources.Berries.MaxValue += Amount;
		return;
	case Bow:
		PlayerResources.Bow.MaxValue += Amount;
		return;
	case Bread:
		PlayerResources.Bread.MaxValue += Amount;
		return;
	case Brick:
		PlayerResources.Brick.MaxValue += Amount;
		return;
	case Ceramic:
		PlayerResources.Ceramic.MaxValue += Amount;
		return;
	case Chainmail:
		PlayerResources.Chainmail.MaxValue += Amount;
		return;
	case ChalkStone:
		PlayerResources.ChalkStone.MaxValue += Amount;
		return;
	case Cheese:
		PlayerResources.Cheese.MaxValue += Amount;
		return;
	case Clay:
		PlayerResources.Clay.MaxValue += Amount;
		return;
	case Cloth:
		PlayerResources.Cloth.MaxValue += Amount;
		return;
	case Coal:
		PlayerResources.Coal.MaxValue += Amount;
		return;
	case Crossbow:
		PlayerResources.Crossbow.MaxValue += Amount;
		return;
	case Egg:
		PlayerResources.Egg.MaxValue += Amount;
		return;
	case Fish:
		PlayerResources.AnimalHide.MaxValue += Amount;
		return;
	case Flax:
		PlayerResources.Flax.MaxValue += Amount;
		return;
	case Flour:
		PlayerResources.Flour.MaxValue += Amount;
		return;
	case Fruits:
		PlayerResources.Fruits.MaxValue += Amount;
		return;
	case Gambeson:
		PlayerResources.Gambeson.MaxValue += Amount;
		return;
	case Grain:
		PlayerResources.Grain.MaxValue += Amount;
		return;
	case Honey:
		PlayerResources.Honey.MaxValue += Amount;
		return;
	case Hop:
		PlayerResources.Hop.MaxValue += Amount;
		return;
	case Ice:
		PlayerResources.Ice.MaxValue += Amount;
		return;
	case IronIgnot:
		PlayerResources.IronIgnot.MaxValue += Amount;
		return;
	case IronOreBoulder:
		PlayerResources.IronOreBoulder.MaxValue += Amount;
		return;
	case IronOrePowder:
		PlayerResources.IronOrePowder.MaxValue += Amount;
		return;
	case Javelins:
		PlayerResources.Javelins.MaxValue += Amount;
		return;
	case Leather:
		PlayerResources.Leather.MaxValue += Amount;
		return;
	case Meat:
		PlayerResources.Meat.MaxValue += Amount;
		return;
	case OrePowder:
		PlayerResources.OrePowder.MaxValue += Amount;
		return;
	case Parchment:
		PlayerResources.Parchment.MaxValue += Amount;
		return;
	case Pennies:
		PlayerResources.Pennies.MaxValue += Amount;
		return;
	case Rope:
		PlayerResources.Rope.MaxValue += Amount;
		return;
	case Saddle:
		PlayerResources.Saddle.MaxValue += Amount;
		return;
	case Salt:
		PlayerResources.Salt.MaxValue += Amount;
		return;
	case Sausages:
		PlayerResources.Sausages.MaxValue += Amount;
		return;
	case SilverOreBoulder:
		PlayerResources.SilverOreBoulder.MaxValue += Amount;
		return;
	case SilverOrePowder:
		PlayerResources.SilverOrePowder.MaxValue += Amount;
		return;
	case SlakedLime:
		PlayerResources.SlakedLime.MaxValue += Amount;
		return;
	case Shield:
		PlayerResources.Shield.MaxValue += Amount;
		return;
	case Spear:
		PlayerResources.Spear.MaxValue += Amount;
		return;
	case Sword:
		PlayerResources.Sword.MaxValue += Amount;
		return;
	case TanningAgent:
		PlayerResources.TanningAgent.MaxValue += Amount;
		return;
	case Tar:
		PlayerResources.Tar.MaxValue += Amount;
		return;
	case TreeBark:
		PlayerResources.TreeBark.MaxValue += Amount;
		return;
	case Vegetables:
		PlayerResources.Vegetables.MaxValue += Amount;
		return;
	case Vine:
		PlayerResources.Vine.MaxValue += Amount;
		return;
	case Water:
		PlayerResources.Water.MaxValue += Amount;
		return;
	case WoodBeam:
		PlayerResources.WoodBeam.MaxValue += Amount;
		return;
	case WoodPlank:
		PlayerResources.WoodPlank.MaxValue += Amount;
		return;
	case Wool:
		PlayerResources.Wool.MaxValue += Amount;
		return;
	default:
		0.0;
		return;
	}
}

bool UResourceManagerBase::CompareResource(EResources Value, float Amount)
{
	switch (Value)
	{
	case Clay:
		if (PlayerResources.Clay.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Coal:
		if (PlayerResources.Coal.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}	
	case Flax:
		if (PlayerResources.Flax.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case IronOrePowder:
		if (PlayerResources.IronOrePowder.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Leather:
		if (PlayerResources.Leather.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Brick:
		if (PlayerResources.Brick.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Tar:
		if (PlayerResources.Tar.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case WoodBeam:
		if (PlayerResources.WoodBeam.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Wool:
		if (PlayerResources.Wool.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Cloth:
		if (PlayerResources.Cloth.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case OrePowder:
		if (PlayerResources.OrePowder.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Saddle:
		if (PlayerResources.Saddle.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case WoodPlank:
		if (PlayerResources.WoodPlank.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Bow:
		if (PlayerResources.Bow.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Chainmail:
		if (PlayerResources.Chainmail.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Crossbow:
		if (PlayerResources.Crossbow.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Gambeson:
		if (PlayerResources.Gambeson.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Javelins:
		if (PlayerResources.Javelins.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Spear:
		if (PlayerResources.Spear.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Sword:
		if (PlayerResources.Sword.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Shield:
		if (PlayerResources.Shield.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Cheese:
		if (PlayerResources.Cheese.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Fish:
		if (PlayerResources.Fish.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Fruits:
		if (PlayerResources.Fruits.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Grain:
		if (PlayerResources.Grain.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Hop:
		if (PlayerResources.Hop.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Sausages:
		if (PlayerResources.Sausages.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Vegetables:
		if (PlayerResources.Vegetables.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Beer:
		if (PlayerResources.Beer.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Flour:
		if (PlayerResources.Flour.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	case Bread:
		if (PlayerResources.Bread.CurrentValue >= Amount)
		{
			return true;
		}
		else
		{
			return false;
		}
	default:
		break;
	}
	return false;
}

float UResourceManagerBase::GetCurrentResourceAmount(EResources Value)
{
	switch (Value)
	{
	case AnimalHide:
		return PlayerResources.AnimalHide.CurrentValue;
	case BattleAxe:
		return PlayerResources.BattleAxe.CurrentValue;
	case Beer:
		return PlayerResources.Beer.CurrentValue;
	case Berries:
		return PlayerResources.Berries.CurrentValue;
	case Bow:
		return PlayerResources.Bow.CurrentValue;
	case Bread:
		return PlayerResources.Bread.CurrentValue;
	case Brick:
		return PlayerResources.Brick.CurrentValue;
	case Ceramic:
		return PlayerResources.Ceramic.CurrentValue;
	case Chainmail:
		return PlayerResources.Chainmail.CurrentValue;
	case ChalkStone:
		return PlayerResources.ChalkStone.CurrentValue;
	case Cheese:
		return PlayerResources.Cheese.CurrentValue;
	case Clay:
		return PlayerResources.Clay.CurrentValue;
	case Cloth:
		return PlayerResources.Cloth.CurrentValue;
	case Coal:
		return PlayerResources.Coal.CurrentValue;
	case Crossbow:
		return PlayerResources.Crossbow.CurrentValue;
	case Egg:
		return PlayerResources.Egg.CurrentValue;
	case Fish:
		return PlayerResources.AnimalHide.CurrentValue;
	case Flax:
		return PlayerResources.Flax.CurrentValue;
	case Flour:
		return PlayerResources.Flour.CurrentValue;
	case Fruits:
		return PlayerResources.Fruits.CurrentValue;
	case Gambeson:
		return PlayerResources.Gambeson.CurrentValue;
	case Grain:
		return PlayerResources.Grain.CurrentValue;
	case Honey:
		return PlayerResources.Honey.CurrentValue;
	case Hop:
		return PlayerResources.Hop.CurrentValue;
	case Ice:
		return PlayerResources.Ice.CurrentValue;
	case IronIgnot:
		return PlayerResources.IronIgnot.CurrentValue;
	case IronOreBoulder:
		return PlayerResources.IronOreBoulder.CurrentValue;
	case IronOrePowder:
		return PlayerResources.IronOrePowder.CurrentValue;
	case Javelins:
		return PlayerResources.Javelins.CurrentValue;
	case Leather:
		return PlayerResources.Leather.CurrentValue;
	case Meat:
		return PlayerResources.Meat.CurrentValue;
	case OrePowder:
		return PlayerResources.OrePowder.CurrentValue;
	case Parchment:
		return PlayerResources.Parchment.CurrentValue;
	case Pennies:
		return PlayerResources.Pennies.CurrentValue;
	case Rope:
		return PlayerResources.Rope.CurrentValue;
	case Saddle:
		return PlayerResources.Saddle.CurrentValue;
	case Salt:
		return PlayerResources.Salt.CurrentValue;
	case Sausages:
		return PlayerResources.Sausages.CurrentValue;
	case SilverOreBoulder:
		return PlayerResources.SilverOreBoulder.CurrentValue;
	case SilverOrePowder:
		return PlayerResources.SilverOrePowder.CurrentValue;
	case SlakedLime:
		return PlayerResources.SlakedLime.CurrentValue;
	case Shield:
		return PlayerResources.Shield.CurrentValue;
	case Spear:
		return PlayerResources.Spear.CurrentValue;
	case Sword:
		return PlayerResources.Sword.CurrentValue;
	case TanningAgent:
		return PlayerResources.TanningAgent.CurrentValue;
	case Tar:
		return PlayerResources.Tar.CurrentValue;
	case TreeBark:
		return PlayerResources.TreeBark.CurrentValue;
	case Vegetables:
		return PlayerResources.Vegetables.CurrentValue;
	case Vine:
		return PlayerResources.Vine.CurrentValue;
	case Water:
		return PlayerResources.Water.CurrentValue;
	case WoodBeam:
		return PlayerResources.WoodBeam.CurrentValue;
	case WoodPlank:
		return PlayerResources.WoodPlank.CurrentValue;
	case Wool:
		return PlayerResources.Wool.CurrentValue;
	default:
		return 0.0;
	}
	return 0.0;

	/*
	switch (Value)
	{
	case Clay:
		return PlayerResources.Clay.CurrentValue;
	case Coal:
		return PlayerResources.Coal.CurrentValue;
	case Flax:
		return PlayerResources.Flax.CurrentValue;
	case IronOre:
		return PlayerResources.IronOre.CurrentValue;
	case Leather:
		return PlayerResources.Leather.CurrentValue;
	case StoneBrick:
		return PlayerResources.StoneBrick.CurrentValue;
	case Tar:
		return PlayerResources.Tar.CurrentValue;
	case WoodBeam:
		return PlayerResources.WoodBeam.CurrentValue;
	case Wool:
		return PlayerResources.Wool.CurrentValue;
	case Cloth:
		return PlayerResources.Cloth.CurrentValue;
	case OrePowder:
		return PlayerResources.OrePowder.CurrentValue;
	case Saddle:
		return PlayerResources.Saddle.CurrentValue;
	case WoodPlank:
		return PlayerResources.WoodPlank.CurrentValue;
	case Bow:
		return PlayerResources.Bow.CurrentValue;
	case Chainmail:
		return PlayerResources.Chainmail.CurrentValue;
	case Crossbow:
		return PlayerResources.Crossbow.CurrentValue;
	case Gambeson:
		return PlayerResources.Gambeson.CurrentValue;
	case Javelins:
		return PlayerResources.Javelins.CurrentValue;
	case Spear:
		return PlayerResources.Spear.CurrentValue;
	case Sword:
		return PlayerResources.Sword.CurrentValue;
	case WoodenShield:
		return PlayerResources.WoodenShield.CurrentValue;
	case Cheese:
		return PlayerResources.Cheese.CurrentValue;
	case Fish:
		return PlayerResources.Fish.CurrentValue;
	case Fruits:
		return PlayerResources.Fruits.CurrentValue;
	case Grain:
		return PlayerResources.Grain.CurrentValue;
	case Hop:
		return PlayerResources.Hop.CurrentValue;
	case Sausages:
		return PlayerResources.Sausages.CurrentValue;
	case Vegetables:
		return PlayerResources.Vegetables.CurrentValue;
	case Beer:
		return PlayerResources.Beer.CurrentValue;
	case Flour:
		return PlayerResources.Flour.CurrentValue;
	case Bread:
		return PlayerResources.Bread.CurrentValue;
	default:
		return 0.0;
	}
	return 0.0;
	*/
}

float UResourceManagerBase::GetMaxResourceAmount(EResources Value)
{
	switch (Value)
	{
	case AnimalHide:
		return PlayerResources.AnimalHide.MaxValue;
	case BattleAxe:
		return PlayerResources.BattleAxe.MaxValue;
	case Beer:
		return PlayerResources.Beer.MaxValue;
	case Berries:
		return PlayerResources.Berries.MaxValue;
	case Bow:
		return PlayerResources.Bow.MaxValue;
	case Bread:
		return PlayerResources.Bread.MaxValue;
	case Brick:
		return PlayerResources.Brick.MaxValue;
	case Ceramic:
		return PlayerResources.Ceramic.MaxValue;
	case Chainmail:
		return PlayerResources.Chainmail.MaxValue;
	case ChalkStone:
		return PlayerResources.ChalkStone.MaxValue;
	case Cheese:
		return PlayerResources.Cheese.MaxValue;
	case Clay:
		return PlayerResources.Clay.MaxValue;
	case Cloth:
		return PlayerResources.Cloth.MaxValue;
	case Coal:
		return PlayerResources.Coal.MaxValue;
	case Crossbow:
		return PlayerResources.Crossbow.MaxValue;
	case Egg:
		return PlayerResources.Egg.MaxValue;
	case Fish:
		return PlayerResources.AnimalHide.MaxValue;
	case Flax:
		return PlayerResources.Flax.MaxValue;
	case Flour:
		return PlayerResources.Flour.MaxValue;
	case Fruits:
		return PlayerResources.Fruits.MaxValue;
	case Gambeson:
		return PlayerResources.Gambeson.MaxValue;
	case Grain:
		return PlayerResources.Grain.MaxValue;
	case Honey:
		return PlayerResources.Honey.MaxValue;
	case Hop:
		return PlayerResources.Hop.MaxValue;
	case Ice:
		return PlayerResources.Ice.MaxValue;
	case IronIgnot:
		return PlayerResources.IronIgnot.MaxValue;
	case IronOreBoulder:
		return PlayerResources.IronOreBoulder.MaxValue;
	case IronOrePowder:
		return PlayerResources.IronOrePowder.MaxValue;
	case Javelins:
		return PlayerResources.Javelins.MaxValue;
	case Leather:
		return PlayerResources.Leather.MaxValue;
	case Meat:
		return PlayerResources.Meat.MaxValue;
	case OrePowder:
		return PlayerResources.OrePowder.MaxValue;
	case Parchment:
		return PlayerResources.Parchment.MaxValue;
	case Pennies:
		return PlayerResources.Pennies.MaxValue;
	case Rope:
		return PlayerResources.Rope.MaxValue;
	case Saddle:
		return PlayerResources.Saddle.MaxValue;
	case Salt:
		return PlayerResources.Salt.MaxValue;
	case Sausages:
		return PlayerResources.Sausages.MaxValue;
	case SilverOreBoulder:
		return PlayerResources.SilverOreBoulder.MaxValue;
	case SilverOrePowder:
		return PlayerResources.SilverOrePowder.MaxValue;
	case SlakedLime:
		return PlayerResources.SlakedLime.MaxValue;
	case Shield:
		return PlayerResources.Shield.MaxValue;
	case Spear:
		return PlayerResources.Spear.MaxValue;
	case Sword:
		return PlayerResources.Sword.MaxValue;
	case TanningAgent:
		return PlayerResources.TanningAgent.MaxValue;
	case Tar:
		return PlayerResources.Tar.MaxValue;
	case TreeBark:
		return PlayerResources.TreeBark.MaxValue;
	case Vegetables:
		return PlayerResources.Vegetables.MaxValue;
	case Vine:
		return PlayerResources.Vine.MaxValue;
	case Water:
		return PlayerResources.Water.MaxValue;
	case WoodBeam:
		return PlayerResources.WoodBeam.MaxValue;
	case WoodPlank:
		return PlayerResources.WoodPlank.MaxValue;
	case Wool:
		return PlayerResources.Wool.MaxValue;
	default:
		return 0.0;

	}
	return 0.0;
}


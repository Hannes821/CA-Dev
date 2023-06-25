// Code written by Zachary Parker for Feldfeste 

#pragma once


/**
 * ENUMs for buildings
 */

UENUM(BlueprintType)
enum EBuildingType
{
    Communal UMETA(DisplayName = "Communal"),
    Infrastructure  UMETA(DisplayName = "Infrastructure"),
    Agricultural UMETA(DisplayName = "Agricultural"),
    Resources UMETA(DisplayName = "Resources"),
    Artisanry UMETA(DisplayName = "Artisanry"),
    Wooden UMETA(DisplayName = "WoodenFortications"),
    Stone UMETA(DisplayName = "StoneFortications"),
    Warfare UMETA(DisplayName = "Warfare")
};


UENUM(BlueprintType)
enum EBuildingName
{
    AnimalMadow UMETA(DisplayName = "Animal Madow"),
    Armoury UMETA(DisplayName = "Armoury"),
    Bakery UMETA(DisplayName = "Bakery"),
    Banner UMETA(DisplayName = "Banner"),
    BigGatehouse UMETA(DisplayName = "Big Gatehouse"),
    BigTowerRound UMETA(DisplayName = "Big Tower Round"),
    BigTowerSquare UMETA(DisplayName = "Big Tower Square"),
    Blacksmith UMETA(DisplayName = "Blacksmith"),
    BreederStable UMETA(DisplayName = "Breeder Stable"),
    Brewery UMETA(DisplayName = "Brewery"),
    Bridge UMETA(DisplayName = "Bridge"),
    Butchery UMETA(DisplayName = "Butchery"),
    Carpentry UMETA(DisplayName = "Carpentry"),
    Chapel UMETA(DisplayName = "Chapel"),
    ChaulkBurner UMETA(DisplayName = "ChalkBurner"),
    Cheesery UMETA(DisplayName = "Cheesery"),
    Cistern UMETA(DisplayName = "Cistern"),
    ClayPit UMETA(DisplayName = "Clay Pit"),
    Coaler UMETA(DisplayName = "Coaler"),
    CoalPan UMETA(DisplayName = "Coal Pan"),
    Coinery UMETA(DisplayName = "Coinery"),
    DonkeyMill UMETA(DisplayName = "DonkeyMill"),
    DrawBridge UMETA(DisplayName = "Draw Bridge"),
    FarmersBarn UMETA(DisplayName = "Farmer's Barn"),
    Field UMETA(DisplayName = "Field"),
    Fisher UMETA(DisplayName = "Fisher"),
    Granary UMETA(DisplayName = "Granary"),
    HedgeLaying UMETA(DisplayName = "HedgeLaying"),
    House UMETA(DisplayName = "House"),
    Hunter UMETA(DisplayName = "Hunter"),
    IceCellar UMETA(DisplayName = "IceCellar"),
    Lumberjack UMETA(DisplayName = "Lumberjack"),
    MarketSquare UMETA(DisplayName = "Market Square"),
    MiddleTower UMETA(DisplayName = "Middle Tower"),
    MineShaft UMETA(DisplayName = "Mine Shaft"),
    Moat UMETA(DisplayName = "Moat"),
    Monastery UMETA(DisplayName = "Monastery"),
    OxCart UMETA(DisplayName = "Ox Cart"),
    PalisadeGatehouse UMETA(DisplayName = "Palisade Gatehouse"),
    PalisadeTower UMETA(DisplayName = "Palisade Tower"),
    PalisadeWall UMETA(DisplayName = "Palisade Wall"),
    PallisadePlatform UMETA(DisplayName = "PallisadePlatform"),
    Path UMETA(DisplayName = "Path"),
    Pillory UMETA(DisplayName = "Pillory"),
    Portcullis UMETA(DisplayName = "Portcullis"),
    Pottery UMETA(DisplayName = "Pottery"),
    Quarry UMETA(DisplayName = "Quarry"),
    Ropery UMETA(DisplayName = "Ropery"),
    SharpenedPoles UMETA(DisplayName = "SharpenedPoles"),
    SiegeCamp UMETA(DisplayName = "Siege Camp"),
    SmallGatehouse UMETA(DisplayName = "Small Gatehouse"),
    SmallTower UMETA(DisplayName = "Small Tower"),
    Smelter UMETA(DisplayName = "Smelter"),
    Statue UMETA(DisplayName = "Statue"),
    Stockpile UMETA(DisplayName = "Stockpile"),
    StoneKeep UMETA(DisplayName = "Stone Keep"),
    StoneStairway UMETA(DisplayName = "Stone Stairway"),
    StoneWall UMETA(DisplayName = "Stone wall"),
    Street UMETA(DisplayName = "Street"),
    Tailory UMETA(DisplayName = "Tailory"),
    Tannery UMETA(DisplayName = "Tannery"),
    TarKettle UMETA(DisplayName = "Tar Kettle/nose"),
    TarTrench UMETA(DisplayName = "Tar Trench"),
    Tavern UMETA(DisplayName = "Tavern"),
    TowerRoofing UMETA(DisplayName = "Tower Roofing"),
    TrainingArea UMETA(DisplayName = "Training Area"),
    Traphole UMETA(DisplayName = "Traphole"),
    TunnelEntrance UMETA(DisplayName = "TunnelEntrance"),
    TunnelShaft UMETA(DisplayName = "TunnelShaft"),
    WatchdogCage UMETA(DisplayName = "Watchdog Cage"),
    Watermill UMETA(DisplayName = "Watermill"),
    Waterpipe UMETA(DisplayName = "Waterpipe"),
    Weavery UMETA(DisplayName = "Weavery"),
    Well UMETA(DisplayName = "Well"),
    WillowFence UMETA(DisplayName = "Willow Fence"),
    Windmill UMETA(DisplayName = "Windmill"),
    WoodenKeep UMETA(DisplayName = "Wooden Keep"),
    WoodenLadder UMETA(DisplayName = "Wooden Ladder"),

};

UENUM(BlueprintType)
enum EConstructionStatus
{
    Preview UMETA(DisplayName = "Preview"),
    UnderConstruction  UMETA(DisplayName = "Under Construction"),
    Built UMETA(DisplayName = "Built"),
    Damageed UMETA(DisplayName = "Damaged")
};
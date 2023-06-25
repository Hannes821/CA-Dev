// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CastleAge03 : ModuleRules
{
	public CastleAge03(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Networking", "OnlineSubsystem", "OnlineSubsystemUtils" });
    }
}

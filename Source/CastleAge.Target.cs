// Owned by Feldfeste Games Studio.

using UnrealBuildTool;
using System.Collections.Generic;

public class CastleAgeTarget : TargetRules
{
	public CastleAgeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "CastleAge" } );
	}
}

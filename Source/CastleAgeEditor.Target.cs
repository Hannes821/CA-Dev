// Owned by Feldfeste Games Studio.

using UnrealBuildTool;
using System.Collections.Generic;

public class CastleAgeEditorTarget : TargetRules
{
	public CastleAgeEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "CastleAge" } );
	}
}

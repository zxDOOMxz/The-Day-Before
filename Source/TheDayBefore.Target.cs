// Copyright TheDayBefore Project. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TheDayBeforeTarget : TargetRules
{
	public TheDayBeforeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		ExtraModuleNames.Add("TheDayBefore");
	}
}

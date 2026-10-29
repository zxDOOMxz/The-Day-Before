// Copyright TheDayBefore Project. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TheDayBeforeServerTarget : TargetRules
{
	public TheDayBeforeServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		ExtraModuleNames.Add("TheDayBefore");
	}
}

// Copyright TheDayBefore Project. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TheDayBeforeEditorTarget : TargetRules
{
	public TheDayBeforeEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("TheDayBefore");
	}
}

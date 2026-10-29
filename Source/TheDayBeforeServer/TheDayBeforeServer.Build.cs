// Copyright TheDayBefore Project. All Rights Reserved.

using UnrealBuildTool;

public class TheDayBeforeServer : ModuleRules
{
	public TheDayBeforeServer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"TheDayBefore"
		});
	}
}

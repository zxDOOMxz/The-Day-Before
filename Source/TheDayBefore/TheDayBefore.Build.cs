// Copyright TheDayBefore Project. All Rights Reserved.

using UnrealBuildTool;

public class TheDayBefore : ModuleRules
{
	public TheDayBefore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Module headers live in subdirectories (Tactical/, Camera/, etc.), so add the module root to include paths
		PublicIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"NavigationSystem",
			"AIModule",
			"GameplayTasks",
			"Niagara",
			"UMG",
			"Slate",
			"SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"NetCore"
		});

		// For replicated properties
		if (Target.bBuildEditor == false)
		{
			PrivateDependencyModuleNames.Add("OnlineSubsystem");
			PrivateDependencyModuleNames.Add("OnlineSubsystemUtils");
		}
	}
}

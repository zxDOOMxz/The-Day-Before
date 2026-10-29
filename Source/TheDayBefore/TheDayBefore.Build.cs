// Copyright TheDayBefore Project. All Rights Reserved.

using UnrealBuildTool;

public class TheDayBefore : ModuleRules
{
	public TheDayBefore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitSharedPCHs;

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
			"SlateCore",
			"GameplayAbilities",
			"GameplayTags"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"NetCore"
		});

		// FPS Camera specific
		PublicDependencyModuleNames.Add("CameraCalibrationCore");
		
		// For replicated properties
		if (Target.bBuildEditor == false)
		{
			PrivateDependencyModuleNames.Add("OnlineSubsystem");
			PrivateDependencyModuleNames.Add("OnlineSubsystemUtils");
		}
	}
}

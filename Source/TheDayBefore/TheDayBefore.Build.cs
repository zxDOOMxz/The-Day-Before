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
			"SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayAbilities",
			"GameplayTags",
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

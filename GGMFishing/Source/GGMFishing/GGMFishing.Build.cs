// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GGMFishing : ModuleRules
{
	public GGMFishing(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange( new string[] { "GGMFishing" });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}

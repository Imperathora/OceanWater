// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BA_Waldmann : ModuleRules
{
	public BA_Waldmann(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}

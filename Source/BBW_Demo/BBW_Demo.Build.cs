// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BBW_Demo : ModuleRules
{
	public BBW_Demo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}

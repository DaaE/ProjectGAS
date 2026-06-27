// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SayuEditor : ModuleRules
{
	public SayuEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"UnrealEd",
			"UMG",
			"UMGEditor",
			"Blutility",
			"AssetRegistry"
		});
	}
}
// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DungeonRoguelike : ModuleRules
{
	public DungeonRoguelike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
        PublicDependencyModuleNames.Add("GeneratorDungeon");
        PublicDependencyModuleNames.Add("GameCore");
        PublicDependencyModuleNames.Add("MathCombat");


    }
}

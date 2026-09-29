using UnrealBuildTool;

public class GameCore : ModuleRules
{
    public GameCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "NavigationSystem",
            "AIModule",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}

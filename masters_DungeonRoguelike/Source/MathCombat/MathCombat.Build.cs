using UnrealBuildTool;

public class MathCombat : ModuleRules
{
    public MathCombat(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "UMG",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}

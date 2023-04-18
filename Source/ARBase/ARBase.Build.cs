using UnrealBuildTool;

public class ARBase : ModuleRules
{
    public ARBase(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "AutomationTest",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UMG",
            }
        );

        PublicIncludePaths.AddRange(new string[] { "ARBase" });
    }
}
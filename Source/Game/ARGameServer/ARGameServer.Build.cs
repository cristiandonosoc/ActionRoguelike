using UnrealBuildTool;

public class ARGameServer : ModuleRules
{
    public ARGameServer(ReadOnlyTargetRules Target) : base(Target)
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
                "CoreUObject",
                "Engine",
            }
        );

        PublicDependencyModuleNames.AddRange(new string[] { "ARBase" });
        PublicIncludePaths.AddRange(new string[] { "Game/ARGameServer" });
    }
}
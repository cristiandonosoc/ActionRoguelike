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

		PrivateDefinitions.AddRange(ARBuildRules.GetModuleDefines(Target.Type));

		if (ARBuildRules.HasClientCode(Target.Type))
		{
			PublicDependencyModuleNames.AddRange(new string[] { "ARGameClient" });
		}

		if (ARBuildRules.HasServerCode(Target.Type))
		{
			PublicDependencyModuleNames.AddRange(new string[] { "ARGameServer" });
		}
	}
}
using UnrealBuildTool;

public class ARGameClient : ModuleRules
{
	public ARGameClient(ReadOnlyTargetRules Target) : base(Target)
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
				"Slate",
				"SlateCore"
			}
		);

		PublicDependencyModuleNames.AddRange(new string[] { "ARBase" });
		PublicIncludePaths.AddRange(new string[] { "Game/ARGameClient" });
	}
}
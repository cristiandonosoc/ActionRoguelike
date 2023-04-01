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
				"UMG"
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

		PrivateDefinitions.AddRange(ARBuildRules.GetClientDefines());
		

		PublicDependencyModuleNames.AddRange(new string[] { "ARBase" });
		PublicIncludePaths.AddRange(new string[] { "Game/ARGameClient" });

		// Only for Client/Server split, we create a circularly dependency.
		// We also tell UBT that we're OK with it.
		PublicDependencyModuleNames.Add("ARGame");
		CircularlyReferencedDependentModules.Add("ARGame");
	}
}
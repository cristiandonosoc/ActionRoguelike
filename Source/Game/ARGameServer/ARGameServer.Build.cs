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
		
		PrivateDefinitions.AddRange(ARBuildRules.GetServerDefines());

		PublicDependencyModuleNames.AddRange(new string[] { "ARBase", "ARGame" });
		PublicIncludePaths.AddRange(new string[] { "Game/ARGameServer" });
		
		// Only for Client/Server split, we create a circularly dependency.
		// We also tell UBT that we're OK with it.
		PublicDependencyModuleNames.Add("ARGame");
		CircularlyReferencedDependentModules.Add("ARGame");
	}
}
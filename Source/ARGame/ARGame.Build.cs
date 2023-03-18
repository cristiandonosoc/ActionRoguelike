// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ARGame : ModuleRules
{
	public ARGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Unreal deps.
		PublicDependencyModuleNames.AddRange(new string[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "GameplayTags", "GameplayTasks", "UMG" });
		PrivateDependencyModuleNames.AddRange(new string[] { });

		// ARGame deps.
		PublicDependencyModuleNames.AddRange(new string[]{"ARBase"});

		PublicIncludePaths.AddRange(new string[] { "ARGame" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

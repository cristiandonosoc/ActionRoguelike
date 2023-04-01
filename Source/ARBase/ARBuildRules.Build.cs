using System.Collections.Generic;
using UnrealBuildTool;

// ARBuildRules defines static functions to reason about how the game and its target are built. 
public class ARBuildRules
{
	// HasServerCode defines whether the target should have server code within it.
	public static bool HasServerCode(TargetType Target)
	{
		switch (Target)
		{
			case TargetType.Editor:
			case TargetType.Game:
			case TargetType.Server:
				return true;
			default:
				return false;
		}
	}

	// HasClientCode defines whether the target should have client code within it.
	public static bool HasClientCode(TargetType Target)
	{
		switch (Target)
		{
			case TargetType.Editor:
			case TargetType.Game:
			case TargetType.Client:
				return true;
			default:
				return false;
		}
	}

	// GetClientDefines returns the macro DEFINES only for client builds.
	public static string[] GetClientDefines()
	{
		return new string[] { "AR_BUILD_CLIENT" };
	}

	public static string[] GetServerDefines()
	{
		return new string[] { "AR_BUILD_SERVER" };
	}

	// GetModuleDefines return the list of defines that should be added to this module.
	public static string[] GetModuleDefines(TargetType Target)
	{
		var defines = new List<string>();

		if (HasClientCode(Target))
		{
			defines.AddRange(GetClientDefines());
		}

		if (HasServerCode(Target))
		{
			defines.AddRange(GetServerDefines());
		}

		return defines.ToArray();
	}
}
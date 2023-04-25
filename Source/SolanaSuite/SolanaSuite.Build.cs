// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SolanaSuite : ModuleRules
{
	public SolanaSuite(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"SeedVault",
			"WalletAdapter",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});
	}
}

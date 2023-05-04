//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

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

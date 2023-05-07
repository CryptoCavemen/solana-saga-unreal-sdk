//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

using UnrealBuildTool;

public class UnrealWalletAdapter : ModuleRules
{
	public UnrealWalletAdapter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"WalletAdapter",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});
	}
}

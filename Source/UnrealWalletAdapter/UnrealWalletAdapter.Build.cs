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
	
		PublicDefinitions.AddRange(
			new string[] {
				"ED25519_CUSTOMRNG",
				"ED25519_CUSTOMHASH",
				"ED25519_NO_SEED"
			}
		);
		
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS");
		}
		
		bEnableUndefinedIdentifierWarnings = false;
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"OpenSSL",
			"WalletAdapter"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"Json",
			"JsonUtilities",
			"HTTP"
		});
	}
}

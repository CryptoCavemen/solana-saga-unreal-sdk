//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

using UnrealBuildTool;
using System.IO;

public class SeedVault : ModuleRules
{
	public SeedVault(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] 
			{
			});
				
		
		PrivateIncludePaths.AddRange(new string[] 
			{
			});
			
		
		PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core",				
			});
			
		
		PrivateDependencyModuleNames.AddRange(new string[]
			{
				"CoreUObject",	
				"Engine",
				"Slate",
				"SlateCore",
			});
		
		
		DynamicallyLoadedModuleNames.AddRange(new string[]
			{
			});


		PublicIncludePathModuleNames.AddRange(new string[]	
			{
				"Launch"
			});
		
		if (Target.IsInPlatformGroup(UnrealPlatformGroup.Android) )
		{
			string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "SeedVault_UPL.xml"));			
			//PublicIncludePaths.Add("$(ModuleDir)/Public/Android");
			//PrivateIncludePaths.Add("ElectraPlayerRuntime/Private/Runtime/Decoder/Android");
		}		

	}
}

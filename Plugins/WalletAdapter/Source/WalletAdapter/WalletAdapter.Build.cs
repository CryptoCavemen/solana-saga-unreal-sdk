// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class WalletAdapter : ModuleRules
{
	public WalletAdapter(ReadOnlyTargetRules Target) : base(Target)
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
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "WalletAdapter_UPL.xml"));			
			//PublicIncludePaths.Add("$(ModuleDir)/Public/Android");
			//PrivateIncludePaths.Add("ElectraPlayerRuntime/Private/Runtime/Decoder/Android");
		}		

	}
}

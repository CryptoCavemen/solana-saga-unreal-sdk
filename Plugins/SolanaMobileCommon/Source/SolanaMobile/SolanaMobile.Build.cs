//
// Copyright (c) 2023 Solana Mobile Inc.
// Author: Sergey Makovkin (makovkin.s@gmail.com)
//

using UnrealBuildTool;

public class SolanaMobile : ModuleRules
{
	public SolanaMobile(ReadOnlyTargetRules Target) : base(Target)
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
				"Core"
			});
			
		
		PrivateDependencyModuleNames.AddRange(new string[]
			{
				"CoreUObject",	
				"Engine"
			});
		
		
		DynamicallyLoadedModuleNames.AddRange(new string[]
			{
			});


		PublicIncludePathModuleNames.AddRange(new string[]	
			{
				"Launch"
			});
	}
}

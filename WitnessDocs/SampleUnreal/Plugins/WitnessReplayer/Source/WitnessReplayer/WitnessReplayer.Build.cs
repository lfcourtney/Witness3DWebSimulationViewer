// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WitnessReplayer : ModuleRules
{
	public WitnessReplayer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "WitnessManagers", "CoreUObject", "Engine", "XmlParser", "InputCore", "UMG", "Slate", "HeadMountedDisplay","SlateCore", "ProceduralMeshComponent", "DTControllers","TimeCaculator"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "WitnessManagers", "CoreUObject", "Engine", "XmlParser", "InputCore", "UMG", "Slate", "HeadMountedDisplay","SlateCore","ProceduralMeshComponent", "DTControllers","TimeCaculator"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}

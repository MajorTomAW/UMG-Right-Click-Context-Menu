// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UMGRightClickContextMenus : ModuleRules
{
	public UMGRightClickContextMenus(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange( new []
		{
			"Core",
			"UMG",
			"GameplayTags",
		});
			
		
		PrivateDependencyModuleNames.AddRange( new []
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
		});
	}
}

// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BlueprintToolEditor : ModuleRules
{
	public BlueprintToolEditor(ReadOnlyTargetRules Target) : base(Target)
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
				"BlueprintToolEditor/Public",
				"BlueprintToolEditor/Public/AssetEditor",
				"BlueprintToolEditor/Public/AssetEditor/Architect",
				"BlueprintToolEditor/Public/Blueprint",
				"BlueprintToolEditor/Public/Blueprint/Core",
				"BlueprintToolEditor/Public/BlueprintEditor",
				"BlueprintToolEditor/Public/BlueprintEditor/Architect",
				"BlueprintToolEditor/Public/BlueprintEditor/ConnectionDrawingPolicy",
				"BlueprintToolEditor/Public/BlueprintEditor/GraphNode",
				"BlueprintToolEditor/Public/BlueprintEditor/GraphPin",
				"BlueprintToolEditor/Public/Details",
				"BlueprintToolEditor/Public/Factory",
				"BlueprintToolEditor/Public/Factory/Blueprint",
				"BlueprintToolEditor/Public/Style",
				"BlueprintToolEditor/Public/Viewport",
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"InputCore",
				"UnrealEd",
				"AssetTools",
				"BlueprintToolRuntime",
				"AdvancedPreviewScene",
				"EditorStyle",
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

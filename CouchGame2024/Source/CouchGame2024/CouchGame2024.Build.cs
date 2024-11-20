// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CouchGame2024 : ModuleRules
{
	public CouchGame2024(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "DeveloperSettings", "Blutility", "EditorScriptingUtilities", "UnrealEd", "EditorSubsystem", "PhysicsCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "GeometryFramework", "LocalMultiplayer", "CableComponent", "EditorScriptingUtilities", "UnrealEd", "UMGEditor" ,"UMG" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

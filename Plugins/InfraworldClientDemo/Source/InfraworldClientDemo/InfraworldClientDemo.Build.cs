// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InfraworldClientDemo : ModuleRules
{
	public InfraworldClientDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicIncludePaths.AddRange( new string[] { "InfraworldClientDemo/Public" });
		PrivateIncludePaths.AddRange( new string[] { "InfraworldClientDemo/Private" });
		PublicDependencyModuleNames.AddRange( new string[] { "Core", "InfraworldRuntime" });
		PrivateDependencyModuleNames.AddRange( new string[] {"CoreUObject", "Engine", "Slate", "SlateCore" });
	}
}

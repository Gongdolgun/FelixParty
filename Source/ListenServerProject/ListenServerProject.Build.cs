// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ListenServerProject : ModuleRules
{
	public ListenServerProject(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.Add("Core");

        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("InputCore");
        PrivateDependencyModuleNames.Add("EnhancedInput");

        PublicDependencyModuleNames.Add("Niagara");
        PublicDependencyModuleNames.Add("OnlineSubsystem");
        PublicDependencyModuleNames.Add("OnlineSubsystemUtils");
        PublicDependencyModuleNames.Add("OnlineSubsystemSteam");
        PublicDependencyModuleNames.Add("AdvancedSessions");
        PublicDependencyModuleNames.Add("AdvancedSteamSessions");

        PublicDependencyModuleNames.Add("MotionWarping");

    }
}
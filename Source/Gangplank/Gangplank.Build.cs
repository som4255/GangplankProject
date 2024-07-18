// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Gangplank : ModuleRules
{
	public Gangplank(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem",
            "UMG",
            "CinematicCamera",
            "EnhancedInput",
            "Niagara",
            "MovieScene",
            "LevelSequence",
            "MediaAssets",
            "Networking",
            "Sockets",
            "ProceduralMeshComponent",
            "Foliage",
            "PhysicsCore",
            "AnimGraphRuntime"
        });
	}
}

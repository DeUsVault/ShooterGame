// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

/*
	@TODO: Total Project
	- CommonUI to replace slate
	- Shader based UI Elements
	- Add EOS Target
	- Add Steam Target
	- Replace Respawning System with Restart Actor for GC
	- Add Defusal GameType
	- Add CTF GameType
	- Change StartFire / StopFire to work UT Style
	- Rollback Lagcomp
	- Rework hitscan netcode based on Vori Blog
	- Performance Optimization
	- Statetree driven AI
	- Multithreaded rendering with RHI Thread
	- Melee
*/

public class ShooterGame : ModuleRules
{
	public ShooterGame(ReadOnlyTargetRules Target) : base(Target)
	{
		// Auto include for Public / Private, since no split
		PublicIncludePaths.Add(ModuleDirectory);

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = false;

		PublicDefinitions.Add("HOST_ONLINE_GAMEMODE_ENABLED=1");
		PublicDefinitions.Add("JOIN_ONLINE_GAME_ENABLED=1");
		PublicDefinitions.Add("INVITE_ONLINE_GAME_ENABLED=1");
		PublicDefinitions.Add("ONLINE_STORE_ENABLED=1");

        PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"OnlineSubsystem",
				"OnlineSubsystemUtils",
				"NavigationSystem",
				"AIModule",
				"GameplayTasks"
			}
		);

        PrivateDependencyModuleNames.AddRange(
			new string[] {
				"EnhancedInput",
				"InputCore", // We don't need this anymore but a few silly things rely on FKey.
				"Slate",
				"SlateCore",
				"ReplicationGraph",
				"PhysicsCore"
			}
		);
		
		/*
			Addtional modules that may be required at run-time.
			Does not include headers at link time, instead is treated
			as an external .dll and is loaded when needed. If modules
			are missing then code will fail out.
		*/
		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
				"OnlineSubsystemNull",
				"NetworkReplayStreaming",
				"NullNetworkReplayStreaming",
				"HttpNetworkReplayStreaming",
				"LocalFileNetworkReplayStreaming"
			}
		);

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"NetworkReplayStreaming"
			}
		);

		if (Target.bBuildDeveloperTools || (Target.Configuration != UnrealTargetConfiguration.Shipping && Target.Configuration != UnrealTargetConfiguration.Test))
        {
            PrivateDependencyModuleNames.Add("GameplayDebugger");
            PublicDefinitions.Add("WITH_GAMEPLAY_DEBUGGER=1");
        }
		else
		{
			PublicDefinitions.Add("WITH_GAMEPLAY_DEBUGGER=0");
		}
		
		if (Target.Version.MajorVersion >= 5 || (Target.Version.MajorVersion == 4 && Target.Version.MinorVersion >= 27))
		{
			PrivateDependencyModuleNames.Add("GameplayCameras");
		}
	}
}

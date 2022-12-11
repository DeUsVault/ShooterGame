// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShooterAutomation : ModuleRules
{
	public ShooterAutomation(ReadOnlyTargetRules Target) : base(Target)
	{
		// Auto include for Public / Private, since no split
		PublicIncludePaths.Add(ModuleDirectory);

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = false;

        PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine"
			}
		);

        PrivateDependencyModuleNames.AddRange(
			new string[] {
				"EnhancedInput",
				"ShooterGame",
				"Gauntlet",
				"OnlineSubsystem",
				"OnlineSubsystemUtils",
				"Slate",
				"SlateCore"
			}
		);
	}
}

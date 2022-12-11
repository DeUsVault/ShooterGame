// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShooterGameData.generated.h"

class UInputMappingContext;
class UInputAction;
class AShooterCharacter;
class AShooterHUD;
class AShooterPlayerController;
class AShooterPlayerState;
class AShooterSpectatorPawn;
class AShooterGameState;
class AShooterDemoSpectator;

/*
	UShooterGameData.
	Non-mutable data asset that contains global game data.
*/
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Shooter Game Data", ShortTooltip = "Data asset containing global game data."))
class UShooterGameData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// Returns the loaded game data.
	static const UShooterGameData& Get();

//////////////////////////////////////////////////////////////////////
//	INPUT ACTIONS (ENHANCED INPUT)
//////////////////////////////////////////////////////////////////////

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputMappingContext> DesktopInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionLook;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionJump;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionSprint;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionCrouch;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionFire;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionAimDownSights;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionCycleWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionReload;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionShowScoreboard;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionPushToTalk;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionEscapeMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionChatTeam;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Game")
	TSoftObjectPtr<UInputAction> InputActionChatGlobal;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Developer")
	TSoftObjectPtr<UInputMappingContext> DeveloperInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Developer")
	TSoftObjectPtr<UInputAction> InputActionSuicide;

//////////////////////////////////////////////////////////////////////
//	GAMEMODE CLASSES
//////////////////////////////////////////////////////////////////////

	UPROPERTY(EditDefaultsOnly, Category = "GameMode|DefaultClasses")
	TSoftClassPtr<AShooterCharacter> 		DefaultPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode|DefaultClasses")
	TSoftClassPtr<AShooterCharacter> 		BotPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode|DefaultClasses")
	TSoftClassPtr<AShooterHUD> 				HUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode|DefaultClasses")
	TSoftClassPtr<AShooterPlayerController> PlayerControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode|DefaultClasses")
	TSoftClassPtr<AShooterPlayerState> 		PlayerStateClass;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode|DefaultClasses")
	TSoftClassPtr<AShooterSpectatorPawn> 	SpectatorClass;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode|DefaultClasses")
	TSoftClassPtr<AShooterGameState> 		GameStateClass;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode|DefaultClasses")
	TSoftClassPtr<AShooterDemoSpectator> 	DemoSpectatorClass;

//////////////////////////////////////////////////////////////////////
//	GAME TWEAKABLES
//////////////////////////////////////////////////////////////////////

	// @TODO: Move analog fire from PC here.

	// @TODO: Move fire trigger threshold from PC here.

};

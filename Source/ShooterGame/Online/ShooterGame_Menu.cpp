// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGame_Menu.h"
#include "Online/ShooterGameSession.h"
#include "Player/ShooterPlayerController_Menu.h"

AShooterGame_Menu::AShooterGame_Menu()
{
	PlayerControllerClass = AShooterPlayerController_Menu::StaticClass();
}

void AShooterGame_Menu::RestartPlayer(class AController* NewPlayer)
{
	// don't restart
}

/** Returns game session class to use */
TSubclassOf<AGameSession> AShooterGame_Menu::GetGameSessionClass() const
{
	return AShooterGameSession::StaticClass();
}

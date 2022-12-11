// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGame_Menu.generated.h"

UCLASS()
class SHOOTERGAME_API AShooterGame_Menu : public AGameModeBase
{
	GENERATED_BODY()

	AShooterGame_Menu();

public:

	// Begin AGameModeBase interface
	/** skip it, menu doesn't require player start or pawn */
	virtual void RestartPlayer(class AController* NewPlayer) override;

	/** Returns game session class to use */
	virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;
	// End AGameModeBase interface

protected:

	/** Perform some final tasks before hosting/joining a session. Remove menus, set king state etc */
	void BeginSession();
	
	/** Display a loading screen */
	void ShowLoadingScreen();
};

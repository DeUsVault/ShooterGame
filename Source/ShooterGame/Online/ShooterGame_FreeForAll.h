// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Online/ShooterGameMode.h"
#include "ShooterGame_FreeForAll.generated.h"

class AShooterPlayerState;

UCLASS()
class SHOOTERGAME_API AShooterGame_FreeForAll : public AShooterGameMode
{
	GENERATED_BODY()
public:

	AShooterGame_FreeForAll();

protected:

	/** best player */
	UPROPERTY(transient)
	AShooterPlayerState* WinnerPlayerState;

	/** check who won */
	virtual void DetermineMatchWinner() override;

	/** check if PlayerState is a winner */
	virtual bool IsWinner(AShooterPlayerState* PlayerState) const override;
};

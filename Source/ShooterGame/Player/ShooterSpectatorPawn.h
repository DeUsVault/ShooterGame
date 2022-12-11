// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/SpectatorPawn.h"
#include "ShooterSpectatorPawn.generated.h"

UCLASS(config = Game, Blueprintable, BlueprintType)
class SHOOTERGAME_API AShooterSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
public:

	AShooterSpectatorPawn();

	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
	/** Multi-directional movement handling (WASD) */
	void Move(const FInputActionValue& Value);

	/** Bi-directional camera movement handling (Mouse) */
	void Look(const FInputActionValue& Value);
};

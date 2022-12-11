// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterCharacterMovement.generated.h"

/*
	Movement component meant for use with Pawns.
*/
UCLASS()
class SHOOTERGAME_API UShooterCharacterMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:

	virtual float GetMaxSpeed() const override;
};


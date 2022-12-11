// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterPickup.h"
#include "ShooterPickup_Health.generated.h"

class AShooterCharacter;

// A pickup object that replenishes character health
UCLASS(Abstract, Blueprintable)
class SHOOTERGAME_API AShooterPickup_Health : public AShooterPickup
{
	GENERATED_BODY()
public:

	AShooterPickup_Health();

	/** check if pawn can use this pickup */
	virtual bool CanBePickedUp(AShooterCharacter* TestPawn) const override;

protected:

	/** how much health does it give? */
	UPROPERTY(EditDefaultsOnly, Category=Pickup)
	int32 Health;

	/** give pickup */
	virtual void GivePickupTo(AShooterCharacter* Pawn) override;
};

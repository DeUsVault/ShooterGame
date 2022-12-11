// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "ShooterLocalPlayer.generated.h"

UCLASS(config=Engine, transient)
class SHOOTERGAME_API UShooterLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
public:

	virtual void SetControllerId(int32 NewControllerId) override;

	virtual FString GetNickname() const;

	class UShooterPersistentUser* GetPersistentUser() const;
	
	/** Initializes the PersistentUser */
	void LoadPersistentUser();

private:
	/** Persistent user data stored between sessions (i.e. the user's savegame) */
	UPROPERTY()
	class UShooterPersistentUser* PersistentUser;
};




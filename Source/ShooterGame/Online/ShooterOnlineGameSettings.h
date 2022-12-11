// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"

/**
 * General session settings for a Shooter game
 */
class SHOOTERGAME_API FShooterOnlineSessionSettings : public FOnlineSessionSettings
{
public:

	FShooterOnlineSessionSettings(bool bIsLAN = false, bool bIsPresence = false, int32 MaxNumPlayers = 4);
	virtual ~FShooterOnlineSessionSettings() {}
};

/**
 * General search setting for a Shooter game
 */
class SHOOTERGAME_API FShooterOnlineSearchSettings : public FOnlineSessionSearch
{
public:
	FShooterOnlineSearchSettings(bool bSearchingLAN = false, bool bSearchingPresence = false);

	virtual ~FShooterOnlineSearchSettings() {}
};

/**
 * Search settings for an empty dedicated server to host a match
 */
class SHOOTERGAME_API FShooterOnlineSearchSettingsEmptyDedicated : public FShooterOnlineSearchSettings
{
public:
	FShooterOnlineSearchSettingsEmptyDedicated(bool bSearchingLAN = false, bool bSearchingPresence = false);

	virtual ~FShooterOnlineSearchSettingsEmptyDedicated() {}
};

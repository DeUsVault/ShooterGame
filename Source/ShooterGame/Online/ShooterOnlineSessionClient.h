// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionClient.h"
#include "ShooterOnlineSessionClient.generated.h"

UCLASS(Config = Game)
class SHOOTERGAME_API UShooterOnlineSessionClient : public UOnlineSessionClient
{
	GENERATED_BODY()
public:

	virtual void OnSessionUserInviteAccepted(
		const bool							bWasSuccess,
		const int32							ControllerId,
		TSharedPtr< const FUniqueNetId >	UserId,
		const FOnlineSessionSearchResult &	InviteResult
	) override;

};

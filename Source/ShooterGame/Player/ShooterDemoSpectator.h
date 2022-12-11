// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterDemoSpectator.generated.h"

UCLASS(config=Game)
class SHOOTERGAME_API AShooterDemoSpectator : public APlayerController
{
	GENERATED_BODY()
public:

	AShooterDemoSpectator();

	virtual void SetupInputComponent() override;
	virtual void SetPlayer( UPlayer* Player ) override;
	virtual void Destroyed() override;

	void OnIncreasePlaybackSpeed();
	void OnDecreasePlaybackSpeed();

	int32 PlaybackSpeed;
};


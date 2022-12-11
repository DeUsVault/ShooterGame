// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ShooterPlayerCameraManager.generated.h"

UCLASS()
class SHOOTERGAME_API AShooterPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:

	AShooterPlayerCameraManager();

	/** normal FOV */
	float NormalFOV;

	/** targeting FOV */
	float TargetingFOV;

	/** After updating camera, inform pawn to update 1p mesh to match camera's location&rotation */
	virtual void UpdateCamera(float DeltaTime) override;
};

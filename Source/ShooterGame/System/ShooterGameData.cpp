// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameData.h"
#include "System/ShooterAssetManager.h"

#include "InputAction.h"
#include "InputMappingContext.h"

const UShooterGameData& UShooterGameData::Get()
{
	return UShooterAssetManager::Get().GetGameData();
}
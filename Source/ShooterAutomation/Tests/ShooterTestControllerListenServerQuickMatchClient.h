// Copyright Epic Games, Inc.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterTestControllerBase.h"
#include "ShooterTestControllerListenServerQuickMatchClient.generated.h"

UCLASS()
class SHOOTERAUTOMATION_API UShooterTestControllerListenServerQuickMatchClient : public UShooterTestControllerBase
{
	GENERATED_BODY()

protected:
	virtual void OnTick(float TimeDelta) override;
};
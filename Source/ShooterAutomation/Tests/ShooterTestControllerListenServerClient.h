// Copyright Epic Games, Inc.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterTestControllerBase.h"
#include "ShooterTestControllerListenServerClient.generated.h"

UCLASS()
class SHOOTERAUTOMATION_API UShooterTestControllerListenServerClient : public UShooterTestControllerBase
{
	GENERATED_BODY()

protected:
	virtual void OnTick(float TimeDelta) override;
};
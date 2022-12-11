// Copyright Epic Games, Inc.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Tests/ShooterTestControllerBase.h"
#include "ShooterTestControllerBasicDedicatedServerTest.generated.h"

UCLASS()
class SHOOTERAUTOMATION_API UShooterTestControllerBasicDedicatedServerTest : public UShooterTestControllerBase
{
	GENERATED_BODY()

protected:
	virtual void OnTick(float TimeDelta) override;

public:
	virtual void OnPostMapChange(UWorld* World) override;
};
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController_Menu.generated.h"

UCLASS()
class SHOOTERGAME_API AShooterPlayerController_Menu : public APlayerController
{
	GENERATED_BODY()
public:

	/** After game is initialized */
	virtual void PostInitializeComponents() override;
};


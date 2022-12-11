// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/ShooterCharacter.h"
#include "ShooterBot.generated.h"

UCLASS()
class SHOOTERGAME_API AShooterBot : public AShooterCharacter
{
	GENERATED_BODY()
public:

	AShooterBot(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category=Behavior)
	class UBehaviorTree* BotBehavior;

	virtual bool IsFirstPerson() const override;

	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;
};
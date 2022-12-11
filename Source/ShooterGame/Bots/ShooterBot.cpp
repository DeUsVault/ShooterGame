// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterBot.h"
#include "Bots/ShooterAIController.h"

AShooterBot::AShooterBot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIControllerClass = AShooterAIController::StaticClass();

	UpdatePawnMeshes();

	bUseControllerRotationYaw = true;
}

bool AShooterBot::IsFirstPerson() const
{
	return false;
}

void AShooterBot::FaceRotation(FRotator NewRotation, float DeltaTime)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 8.0f);

	Super::FaceRotation(CurrentRotation, DeltaTime);
}

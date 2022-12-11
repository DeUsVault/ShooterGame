// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterDemoSpectator.h"

#include "Engine/DemoNetDriver.h"

AShooterDemoSpectator::AShooterDemoSpectator()
{
	bShowMouseCursor = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	bShouldPerformFullTickWhenPaused = true;
}

void AShooterDemoSpectator::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction( "NextWeapon", IE_Pressed, this, &AShooterDemoSpectator::OnIncreasePlaybackSpeed );
	InputComponent->BindAction( "PrevWeapon", IE_Pressed, this, &AShooterDemoSpectator::OnDecreasePlaybackSpeed );
}

void AShooterDemoSpectator::SetPlayer( UPlayer* InPlayer )
{
	Super::SetPlayer( InPlayer );

	FActorSpawnParameters SpawnInfo;

	SpawnInfo.Owner				= this;
	SpawnInfo.Instigator		= GetInstigator();
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PlaybackSpeed = 2;

	//FInputModeGameAndUI InputMode;
	//InputMode.SetWidgetToFocus(DemoHUD);
	//
	//SetInputMode(InputMode);
}

static float PlaybackSpeedLUT[5] = { 0.1f, 0.5f, 1.0f, 2.0f, 4.0f };

void AShooterDemoSpectator::OnIncreasePlaybackSpeed()
{
	PlaybackSpeed = FMath::Clamp( PlaybackSpeed + 1, 0, 4 );

	GetWorldSettings()->DemoPlayTimeDilation = PlaybackSpeedLUT[ PlaybackSpeed ];
}

void AShooterDemoSpectator::OnDecreasePlaybackSpeed()
{
	PlaybackSpeed = FMath::Clamp( PlaybackSpeed - 1, 0, 4 );

	GetWorldSettings()->DemoPlayTimeDilation = PlaybackSpeedLUT[ PlaybackSpeed ];
}

void AShooterDemoSpectator::Destroyed()
{
	Super::Destroyed();
}

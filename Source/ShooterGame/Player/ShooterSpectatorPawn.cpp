// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterSpectatorPawn.h"
#include "System/ShooterGameData.h"

#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AShooterSpectatorPawn::AShooterSpectatorPawn()
{
	bReplicates = false;
}

void AShooterSpectatorPawn::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			UInputMappingContext* DesktopContext = UShooterGameData::Get().DesktopInputMappingContext.LoadSynchronous();

			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DesktopContext, 0);
		}
	}
}

void AShooterSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	auto* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	UInputAction* MoveAction = UShooterGameData::Get().InputActionMove.LoadSynchronous();
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

	UInputAction* LookAction = UShooterGameData::Get().InputActionLook.LoadSynchronous();
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
}

void AShooterSpectatorPawn::Move(const FInputActionValue& Value)
{
	FVector MoveInput = Value.Get<FVector>();

	MoveForward(MoveInput.Y);
	MoveRight(MoveInput.X);
	MoveUp_World(MoveInput.Z);
}

void AShooterSpectatorPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X * BaseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
	AddControllerPitchInput(LookInput.Y * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}
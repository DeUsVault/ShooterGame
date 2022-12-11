// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/*
	Macro header for general purpose macro junk.
	Logging should go in ShooterLogging.h
*/

/** when you modify this, please note that this information can be saved with instances
	* also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list **/
#define COLLISION_WEAPON		ECC_GameTraceChannel1
#define COLLISION_PROJECTILE	ECC_GameTraceChannel2
#define COLLISION_PICKUP		ECC_GameTraceChannel3

#define MAX_PLAYER_NAME_LENGTH 16

#ifndef SHOOTER_SUPPORTS_OFFLINE_SPLIT_SCREEEN
	#define SHOOTER_SUPPORTS_OFFLINE_SPLIT_SCREEEN 1
#endif

// whether the platform will signal a controller pairing change on a controller disconnect. if not, we need to treat the pairing change as a request to switch profiles when the destination profile is not specified
#ifndef SHOOTER_CONTROLLER_PAIRING_ON_DISCONNECT
	#define SHOOTER_CONTROLLER_PAIRING_ON_DISCONNECT 1
#endif

// whether the game should display an account picker when a new input device is connected, while the "please reconnect controller" message is on screen.
#ifndef SHOOTER_CONTROLLER_PAIRING_PROMPT_FOR_NEW_USER_WHEN_RECONNECTING
	#define SHOOTER_CONTROLLER_PAIRING_PROMPT_FOR_NEW_USER_WHEN_RECONNECTING 0
#endif

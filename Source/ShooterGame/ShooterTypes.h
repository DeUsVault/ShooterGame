// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterTypes.generated.h"

class UMaterial;

// This shouldn't even be a flipping enum.
// the is an entire system in GameMode exactly for this using FName.
// Epic why... @TODO: Fix.
namespace EShooterMatchState
{
	enum Type
	{
		Warmup,
		Playing,
		Won,
		Lost,
	};
}

/** keep in sync with ShooterImpactEffect */
// @TODO: This one probably makes sense to be namespaced for expansion.
// but still dunno feels a shoddy way to handle PhysMat types anyways.
// and is doubtful this will play nice with Chaos anyway.
UENUM()
namespace EShooterPhysMaterialType
{
	enum Type
	{
		Unknown,
		Concrete,
		Dirt,
		Water,
		Metal,
		Wood,
		Grass,
		Glass,
		Flesh,
	};
}

#define SHOOTER_SURFACE_Default		SurfaceType_Default
#define SHOOTER_SURFACE_Concrete	SurfaceType1
#define SHOOTER_SURFACE_Dirt		SurfaceType2
#define SHOOTER_SURFACE_Water		SurfaceType3
#define SHOOTER_SURFACE_Metal		SurfaceType4
#define SHOOTER_SURFACE_Wood		SurfaceType5
#define SHOOTER_SURFACE_Grass		SurfaceType6
#define SHOOTER_SURFACE_Glass		SurfaceType7
#define SHOOTER_SURFACE_Flesh		SurfaceType8

USTRUCT()
struct FDecalData
{
	GENERATED_BODY()

	/** material */
	UPROPERTY(EditDefaultsOnly, Category=Decal)
	UMaterial* DecalMaterial;

	/** quad size (width & height) */
	UPROPERTY(EditDefaultsOnly, Category=Decal)
	float DecalSize;

	/** lifespan */
	UPROPERTY(EditDefaultsOnly, Category=Decal)
	float LifeSpan;

	/** defaults */
	FDecalData()
		: DecalMaterial(nullptr)
		, DecalSize(256.f)
		, LifeSpan(10.f)
	{
	}
};
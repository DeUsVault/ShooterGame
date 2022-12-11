// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterTypes.h"
#include "Interfaces/VoiceInterface.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

class FUniqueNetId;

struct FHitData
{
	/** Last hit time. */
	float HitTime;
	
	/** strength of hit icon */
	float HitPercentage;

	/** Initialise defaults. */
	FHitData()
	{
		HitTime = 0.0f;
		HitPercentage = 0.0f;
	}
};

struct FDeathMessage
{
	/** Name of player scoring kill. */
	FString KillerDesc;

	/** Name of killed player. */
	FString VictimDesc;

	/** Killer is local player. */
	uint8 bKillerIsOwner : 1;
	
	/** Victim is local player. */
	uint8 bVictimIsOwner : 1;

	/** Team number of the killer. */
	int32 KillerTeamNum;

	/** Team number of the victim. */
	int32 VictimTeamNum; 

	/** timestamp for removing message */
	float HideTime;

	/** What killed the player. */
	TWeakObjectPtr<class UShooterDamageType> DamageType;

	/** Initialise defaults. */
	FDeathMessage()
		: bKillerIsOwner(false)
		, bVictimIsOwner(false)
		, KillerTeamNum(0)
		, VictimTeamNum(0)		
		, HideTime(0.f)
	{
	}
};

UCLASS()
class SHOOTERGAME_API AShooterHUD : public AHUD
{
	GENERATED_BODY()
public:

	AShooterHUD();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Main HUD update loop. */
	virtual void DrawHUD() override;

	/** 
	 * Set state of current match.
	 *
	 * @param	NewState	The new match state.
	 */
	void SetMatchState(EShooterMatchState::Type NewState);

	/** Get state of current match. */
	EShooterMatchState::Type GetMatchState() const;

	/** Turns off scoreboard if it is being displayed */
	void ConditionalCloseScoreboard(bool bFocus = false);

	/** Toggles scoreboard */
	void ToggleScoreboard();

	/** 
	 * Toggles in game scoreboard.
	 * Note:Will not display if the game menu is visible.

	 * @param	bEnable	Required scoreboard display state.
	 * @param	bFocus	Give keyboard focus to the scoreboard.
	 * @return	true, if the scoreboard visibility changed
	 */
	bool ShowScoreboard(bool bEnable, bool bFocus = false);

	/* Is the match over (IE Is the state Won or Lost). */
	bool IsMatchOver() const;
		
protected:
	/** Floor for automatic hud scaling. */
	static const float MinHudScale;

	/** Reflex Time Accumulator */
	float TimePassed;

	/** Reflex Timers */
	float LatencyTotal, LatencyGame, LatencyRender, Framerate;

	/** When we got last notice about out of ammo. */
	float NoAmmoNotifyTime;

	/** How long notice is fading out. */
	float NoAmmoFadeOutTime;

	/** Most recent hit time, used to check if we need to draw hit indicator at all. */
	float LastHitTime;

	/** How long till hit notify fades out completely. */
	float HitNotifyDisplayTime;

	/** When we last time hit the enemy. */
	float LastEnemyHitTime;

	/** How long till enemy hit notify fades out completely. */
	float LastEnemyHitDisplayTime;

	/** UI scaling factor for other resolutions than Full HD. */
	float ScaleUI;

	/** Current animation pulse value. */
	float PulseValue;

	/** Big "KILLED [PLAYER]" message text above the crosshair. */
	FText CenteredKillMessage;

	/** last time we killed someone. */
	float LastKillTime;

	/** How long the message will fade out. */
	float KillFadeOutTime;

	/** Offsets to display hit indicator parts. */
	FVector2D Offsets[8];

	/** General offset for HUD elements. */
	float Offset;

	/** Runtime data for hit indicator. */
	FHitData HitNotifyData[8];

	/** Active death messages. */
	TArray<FDeathMessage> DeathMessages;

	/** State of match. */
	EShooterMatchState::Type MatchState;

	/** Is the scoreboard widget on screen? */
	uint32 bIsScoreBoardVisible:1;

	/** Called every time game is started. */
	virtual void PostInitializeComponents() override;

	/** 
	 * Converts floating point seconds to MM:SS string.
	 *
	 * @param TimeSeconds		The time to get a string for.
	 */
	FString GetTimeString(float TimeSeconds);

	/** Draw Performance timer */
	void DrawPerfTimer(const FString& Label, const FString& Value, float PosX, float PosY);

	/** Delegate for telling other methods when players have started/stopped talking */
	FOnPlayerTalkingStateChangedDelegate OnPlayerTalkingStateChangedDelegate;
	void OnPlayerTalkingStateChanged(TSharedRef<const FUniqueNetId> TalkingPlayerId, bool bIsTalking);

	/** Temporary helper for drawing text-in-a-box. */
	void DrawDebugInfoString(const FString& Text, float PosX, float PosY, bool bAlignLeft, bool bAlignTop, const FColor& TextColor);

	/*
	 * Create the chat widget if it doesn't already exist.
	 *
	 * @return		true if the widget was created.
	 */
	bool TryCreateChatWidget();

};
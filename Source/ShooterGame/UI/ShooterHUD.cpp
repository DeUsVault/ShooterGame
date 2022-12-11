// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterHUD.h"
#include "Player/ShooterPlayerState.h"
#include "Player/ShooterPlayerController.h"
#include "Weapons/ShooterWeapon.h"
#include "Weapons/ShooterDamageType.h"
#include "Weapons/ShooterWeapon_Instant.h"
#include "Settings/ShooterGameUserSettings.h"
#include "ShooterTypes.h"

#include "Engine/ViewportSplitScreen.h"
#include "Performance/LatencyMarkerModule.h"
#include "Misc/NetworkVersion.h"
#include "Online/CoreOnline.h"
#include "OnlineSubsystemUtils.h"

#define LOCTEXT_NAMESPACE "ShooterGame.HUD.Menu"

const float AShooterHUD::MinHudScale = 0.5f;

AShooterHUD::AShooterHUD()
{
	NoAmmoFadeOutTime =  1.0f;
	HitNotifyDisplayTime = 0.75f;
	KillFadeOutTime = 2.0f;
	LastEnemyHitDisplayTime = 0.2f;
	NoAmmoNotifyTime = -NoAmmoFadeOutTime;
	LastKillTime = - KillFadeOutTime;
	LastEnemyHitTime = -LastEnemyHitDisplayTime;

	TimePassed = 0.0f;
	LatencyTotal = 0, LatencyGame = 0, LatencyRender = 0, Framerate = 0;

	OnPlayerTalkingStateChangedDelegate = FOnPlayerTalkingStateChangedDelegate::CreateUObject(this, &AShooterHUD::OnPlayerTalkingStateChanged);
}

void AShooterHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ConditionalCloseScoreboard(true);

	AShooterPlayerController* ShooterPC = Cast<AShooterPlayerController>(PlayerOwner);
	if (ShooterPC != NULL )
	{
		// Reset the ignore input flags, so we can control the camera during warmup
		ShooterPC->SetCinematicMode(false,false,false,true,true);
	}

	Super::EndPlay(EndPlayReason);
}

void AShooterHUD::SetMatchState(EShooterMatchState::Type NewState)
{
	MatchState = NewState;
}

EShooterMatchState::Type AShooterHUD::GetMatchState() const
{
	return MatchState;
}

FString AShooterHUD::GetTimeString(float TimeSeconds)
{
	// only minutes and seconds are relevant
	const int32 TotalSeconds = FMath::Max(0, FMath::TruncToInt(TimeSeconds) % 3600);
	const int32 NumMinutes = TotalSeconds / 60;
	const int32 NumSeconds = TotalSeconds % 60;

	const FString TimeDesc = FString::Printf(TEXT("%02d:%02d"), NumMinutes, NumSeconds);
	return TimeDesc;
}

void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();
	
}

void AShooterHUD::OnPlayerTalkingStateChanged(TSharedRef<const FUniqueNetId> TalkingPlayerId, bool bIsTalking)
{
	if (bIsScoreBoardVisible)
	{
		//ScoreboardWidget->StoreTalkingPlayerData(TalkingPlayerId.Get(), bIsTalking);
	}
}

void AShooterHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	bIsScoreBoardVisible = false;

	IOnlineSubsystem* const OnlineSub = Online::GetSubsystem(GetWorld());
	if (OnlineSub)
	{
		IOnlineVoicePtr Voice = OnlineSub->GetVoiceInterface();
		if (Voice.IsValid())
		{
			Voice->AddOnPlayerTalkingStateChangedDelegate_Handle(OnPlayerTalkingStateChangedDelegate);
		}
	}
}

void AShooterHUD::ConditionalCloseScoreboard(bool bFocus)
{
	if (bIsScoreBoardVisible)
	{
		ShowScoreboard(false, bFocus);
	}
}

void AShooterHUD::ToggleScoreboard()
{
	ShowScoreboard(!bIsScoreBoardVisible);
}

bool AShooterHUD::ShowScoreboard(bool bEnable, bool bFocus)
{
	if( bIsScoreBoardVisible == bEnable)
	{
		// if the scoreboard is already enabled, disable it in favour of the new request
		if( bEnable )
		{
			ToggleScoreboard();
		}
		else
		{
			return false;
		}
	}
	
	if (bEnable)
	{
		AShooterPlayerController* ShooterPC = Cast<AShooterPlayerController>(PlayerOwner);
		if (ShooterPC == NULL || ShooterPC->IsGameMenuVisible() )
		{
			return false;
		}
	}

	bIsScoreBoardVisible = bEnable;
	if (bIsScoreBoardVisible)
	{
		//SAssignNew(ScoreboardWidgetOverlay,SOverlay)
		//+SOverlay::Slot()
		//.HAlign(EHorizontalAlignment::HAlign_Center)
		//.VAlign(EVerticalAlignment::VAlign_Center)
		//.Padding(FMargin(50))
		//[
		//	SAssignNew(ScoreboardWidget, SShooterScoreboardWidget)
		//		.PCOwner(MakeWeakObjectPtr(PlayerOwner))
		//		.MatchState(GetMatchState())
		//];
		//
		//GEngine->GameViewport->AddViewportWidgetContent(
		//	SAssignNew(ScoreboardWidgetContainer,SWeakWidget)
		//	.PossiblyNullContent(ScoreboardWidgetOverlay));

		if( bFocus )
		{
			// Give input focus to the scoreboard
			//FSlateApplication::Get().SetKeyboardFocus(ScoreboardWidget);
		}
	} 
	else
	{
		//if (ScoreboardWidgetContainer.IsValid())
		//{
		//	if (GEngine && GEngine->GameViewport)
		//	{
		//		GEngine->GameViewport->RemoveViewportWidgetContent(ScoreboardWidgetContainer.ToSharedRef());
		//	}
		//}
		
		if( bFocus )
		{
			// Make sure viewport has focus
			FSlateApplication::Get().SetAllUserFocusToGameViewport();
		}
	}
	return true;
}

bool AShooterHUD::TryCreateChatWidget()
{
	return false;
}

bool AShooterHUD::IsMatchOver() const
{
	return GetMatchState() == EShooterMatchState::Lost || GetMatchState() == EShooterMatchState::Won;
}

#undef LOCTEXT_NAMESPACE

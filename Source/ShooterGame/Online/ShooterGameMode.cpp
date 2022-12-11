// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "Online/ShooterGameSession.h"
#include "Online/ShooterGameState.h"
#include "Player/ShooterSpectatorPawn.h"
#include "Player/ShooterDemoSpectator.h"
#include "Player/ShooterCharacter.h"
#include "Player/ShooterPlayerState.h"
#include "Player/ShooterPlayerController.h"
#include "System/ShooterGameInstance.h"
#include "System/ShooterGameData.h"
#include "Bots/ShooterAIController.h"
#include "Teams/ShooterTeamStart.h"
#include "UI/ShooterHUD.h"

#include "Engine/PlayerStartPIE.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameNetworkManager.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameDelegates.h"
#include "EngineUtils.h"

AShooterGameMode::AShooterGameMode()
{
	MinRespawnDelay = 5.0f;
	bAllowBots = true;	
	bNeedsBotCreation = true;
	bUseSeamlessTravel = FParse::Param(FCommandLine::Get(), TEXT("NoSeamlessTravel")) ? false : true;
}

void AShooterGameMode::PostInitProperties()
{
	Super::PostInitProperties();

	if (!IsTemplate()) // Don't set DemoSpectator for CDO.
	{
		ReplaySpectatorPlayerControllerClass = UShooterGameData::Get().DemoSpectatorClass.LoadSynchronous();
	}

	if (PlatformPlayerControllerClass)
	{
		PlayerControllerClass = PlatformPlayerControllerClass;
	}
}

void AShooterGameMode::InitGameState()
{
	GameState->GameModeClass = GetClass();
	GameState->ReceivedGameModeClass();

	GameState->SpectatorClass = UShooterGameData::Get().SpectatorClass.LoadSynchronous();
	GameState->ReceivedSpectatorClass();
}

FString AShooterGameMode::GetBotsCountOptionName()
{
	return FString(TEXT("Bots"));
}

void AShooterGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	const int32 BotsCountOptionValue = UGameplayStatics::GetIntOption(Options, GetBotsCountOptionName(), 0);
	SetAllowBots(BotsCountOptionValue > 0 ? true : false, BotsCountOptionValue);	
	
	AGameModeBase::InitGame(MapName, Options, ErrorMessage);
	SetMatchState(MatchState::EnteringMap);

	UClass* LoadedGameState = UShooterGameData::Get().GameStateClass.LoadSynchronous();
	checkf(LoadedGameState, TEXT("No GameState class set."));
	checkf(LoadedGameState->IsChildOf<AGameState>(), TEXT("Mixing AGameStateBase with AGameMode is not compatible."));

	// Bind to delegates
	FGameDelegates::Get().GetPendingConnectionLostDelegate().AddUObject(this, &AGameMode::NotifyPendingConnectionLost);
	FGameDelegates::Get().GetPreCommitMapChangeDelegate().AddUObject(this, &AGameMode::PreCommitMapChange);
	FGameDelegates::Get().GetPostCommitMapChangeDelegate().AddUObject(this, &AGameMode::PostCommitMapChange);
	FGameDelegates::Get().GetHandleDisconnectDelegate().AddUObject(this, &AGameMode::HandleDisconnect);

	const UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance && Cast<UShooterGameInstance>(GameInstance)->GetOnlineMode() != EOnlineMode::Offline)
	{
		bPauseable = false;
	}
}

void AShooterGameMode::SetAllowBots(bool bInAllowBots, int32 InMaxBots)
{
	bAllowBots = bInAllowBots;
	MaxBots = InMaxBots;
}

/** Returns game session class to use */
TSubclassOf<AGameSession> AShooterGameMode::GetGameSessionClass() const
{
	return AShooterGameSession::StaticClass();
}

void AShooterGameMode::PreInitializeComponents()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient; // We never want to save game states or network managers into a map									

	checkf(!UShooterGameData::Get().GameStateClass.IsNull(), TEXT("No GameState class set.")); 

	GameState = GetWorld()->SpawnActor<AGameStateBase>(UShooterGameData::Get().GameStateClass.LoadSynchronous(), SpawnInfo);
	GetWorld()->SetGameState(GameState);
	if (GameState)
	{
		GameState->AuthorityGameMode = this;
	}

	// Only need NetworkManager for servers in net games
	AWorldSettings* WorldSettings = GetWorld()->GetWorldSettings();
	GetWorld()->NetworkManager = WorldSettings->GameNetworkManagerClass ? GetWorld()->SpawnActor<AGameNetworkManager>(WorldSettings->GameNetworkManagerClass, SpawnInfo) : nullptr;

	InitGameState();

	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AShooterGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void AShooterGameMode::DefaultTimer()
{
	// don't update timers for Play In Editor mode, it's not real match
	if (GetWorld()->IsPlayInEditor())
	{
		// start match if necessary.
		if (GetMatchState() == MatchState::WaitingToStart)
		{
			StartMatch();
		}
		return;
	}

	AShooterGameState* const MyGameState = Cast<AShooterGameState>(GameState);
	if (MyGameState && MyGameState->RemainingTime > 0 && !MyGameState->bTimerPaused)
	{
		MyGameState->RemainingTime--;
		
		if (MyGameState->RemainingTime <= 0)
		{
			if (GetMatchState() == MatchState::WaitingPostMatch)
			{
				RestartGame();
			}
			else if (GetMatchState() == MatchState::InProgress)
			{
				FinishMatch();

				// Send end round events
				for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
				{
					AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(*It);
					if (PlayerController && MyGameState)
					{
						AShooterPlayerState* PlayerState = Cast<AShooterPlayerState>((*It)->PlayerState);
						const bool bIsWinner = IsWinner(PlayerState);
					
						PlayerController->ClientSendRoundEndEvent(bIsWinner, MyGameState->ElapsedTime);
					}
				}
			}
			else if (GetMatchState() == MatchState::WaitingToStart)
			{
				StartMatch();
			}
		}
	}
}

void AShooterGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();

	if (bNeedsBotCreation)
	{
		CreateBotControllers();
		bNeedsBotCreation = false;
	}

	if (bDelayedStart)
	{
		// start warmup if needed
		AShooterGameState* const MyGameState = Cast<AShooterGameState>(GameState);
		if (MyGameState && MyGameState->RemainingTime == 0)
		{
			if (!GetWorld()->IsPlayInEditor() && WarmupTime > 0) // Skip warmup in PIE.
			{
				MyGameState->RemainingTime = WarmupTime;
			}
			else
			{
				MyGameState->RemainingTime = 0.0f;
			}
		}
	}
}

void AShooterGameMode::HandleMatchHasStarted()
{
	bNeedsBotCreation = true;
	Super::HandleMatchHasStarted();

	AShooterGameState* const MyGameState = Cast<AShooterGameState>(GameState);
	MyGameState->RemainingTime = RoundTime;	
	StartBots();	

	// Notify players
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		if (AShooterPlayerController* PC = Cast<AShooterPlayerController>(*It))
		{
			PC->ClientGameStarted();
		}
	}
}

// Empty function, obviously for example, but i'm thinking people probably know their way around by now.
void AShooterGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void AShooterGameMode::FinishMatch()
{
	AShooterGameState* const MyGameState = Cast<AShooterGameState>(GameState);
	if (IsMatchInProgress())
	{
		EndMatch();
		DetermineMatchWinner();		

		// Notify players
		for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
		{
			AShooterPlayerState* PlayerState = Cast<AShooterPlayerState>((*It)->PlayerState);
			const bool bIsWinner = IsWinner(PlayerState);
			(*It)->GameHasEnded(NULL, bIsWinner);
		}

		// Lock all pawns; pawns are not marked as keep for seamless travel, so we will create new pawns
		// on the next match rather than turning these back on.
		for (APawn* Pawn : TActorRange<APawn>(GetWorld()))
		{
			Pawn->TurnOff();
		}

		// Set up to restart the match
		MyGameState->RemainingTime = TimeBetweenMatches;
	}
}

void AShooterGameMode::RequestFinishAndExitToMainMenu()
{
	FinishMatch();

	if (UShooterGameInstance* GameInstance = Cast<UShooterGameInstance>(GetGameInstance()))
	{
		GameInstance->RemoveSplitScreenPlayers();
	}

	AShooterPlayerController* LocalPrimaryController = nullptr;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AShooterPlayerController* Controller = Cast<AShooterPlayerController>(*Iterator);

		if (!Controller)
		{
			continue;
		}

		if (!Controller->IsLocalController())
		{
			const FText RemoteReturnReason = NSLOCTEXT("NetworkErrors", "HostHasLeft", "Host has left the game.");
			Controller->ClientReturnToMainMenuWithTextReason(RemoteReturnReason);
		}
		else
		{
			LocalPrimaryController = Controller;
		}
	}

	// GameInstance should be calling this from an EndState.  So call the PC function that performs cleanup, not the one that sets GI state.
	if (LocalPrimaryController != nullptr)
	{
		LocalPrimaryController->HandleReturnToMainMenu();
	}
}

void AShooterGameMode::DetermineMatchWinner()
{
	// Nothing to do here
}

bool AShooterGameMode::IsWinner(class AShooterPlayerState* PlayerState) const
{
	return false;
}

void AShooterGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	AShooterGameState* const ShooterGameState = Cast<AShooterGameState>(GameState);
	if(ShooterGameState && ShooterGameState->HasMatchEnded()) // Is MatchIsOver?
	{
		ErrorMessage = TEXT("Match is over!");
	}
	else // GameSession can be nullptr if the match is over
	{
		Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	}
}


void AShooterGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Update spectator location for client
	AShooterPlayerController* NewPC = Cast<AShooterPlayerController>(NewPlayer);
	if (NewPC && !NewPC->GetPawn())
	{
		NewPC->ClientSetSpectatorCamera(NewPC->GetSpawnLocation(), NewPC->GetControlRotation());
	}

	// Notify new player if match is already in progress
	if (NewPC && IsMatchInProgress())
	{
		NewPC->ClientGameStarted();
		NewPC->ClientStartOnlineGame();
	}
}

void AShooterGameMode::Killed(AController* Killer, AController* KilledPlayer, APawn* KilledPawn, const UDamageType* DamageType)
{
	AShooterPlayerState* KillerPlayerState = Killer ? Cast<AShooterPlayerState>(Killer->PlayerState) : nullptr;
	AShooterPlayerState* VictimPlayerState = KilledPlayer ? Cast<AShooterPlayerState>(KilledPlayer->PlayerState) : nullptr;

	if (KillerPlayerState && KillerPlayerState != VictimPlayerState)
	{
		KillerPlayerState->ScoreKill(VictimPlayerState, KillScore);
		KillerPlayerState->InformAboutKill(KillerPlayerState, DamageType, VictimPlayerState);
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->ScoreDeath(KillerPlayerState, DeathScore);
		VictimPlayerState->BroadcastDeath(KillerPlayerState, DamageType, VictimPlayerState);
	}
}

float AShooterGameMode::ModifyDamage(float Damage, AActor* DamagedActor, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const
{
	float ActualDamage = Damage;

	AShooterCharacter* DamagedPawn = Cast<AShooterCharacter>(DamagedActor);
	if (DamagedPawn && EventInstigator)
	{
		AShooterPlayerState* DamagedPlayerState = Cast<AShooterPlayerState>(DamagedPawn->GetPlayerState());
		AShooterPlayerState* InstigatorPlayerState = Cast<AShooterPlayerState>(EventInstigator->PlayerState);

		// Disable friendly fire
		if (!CanDealDamage(InstigatorPlayerState, DamagedPlayerState))
		{
			ActualDamage = 0.0f;
		}

		// Scale self instigated damage
		if (InstigatorPlayerState == DamagedPlayerState)
		{
			ActualDamage *= DamageSelfScale;
		}
	}
	return ActualDamage;
}

bool AShooterGameMode::CanDealDamage(class AShooterPlayerState* DamageInstigator, class AShooterPlayerState* DamagedPlayer) const
{
	return true;
}

bool AShooterGameMode::AllowCheats(APlayerController* P)
{
	return true;
}

bool AShooterGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

void AShooterGameMode::InitializeHUDForPlayer_Implementation(APlayerController* NewPlayer)
{
	// Tell client what HUD class to use
	NewPlayer->ClientSetHUD(UShooterGameData::Get().HUDClass.LoadSynchronous());
}

TSubclassOf<APlayerController> AShooterGameMode::GetPlayerControllerClassToSpawnForSeamlessTravel(APlayerController* PreviousPC)
{
	if (PreviousPC && UShooterGameData::Get().DemoSpectatorClass && PreviousPC->PlayerState 
		&& PreviousPC->PlayerState->IsOnlyASpectator())
	{
		return UShooterGameData::Get().DemoSpectatorClass.LoadSynchronous();
	}
	return UShooterGameData::Get().PlayerControllerClass.LoadSynchronous();
}

APlayerController* AShooterGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	if (Options.Contains(FString(TEXT("SpectatorOnly=1"))) && UShooterGameData::Get().DemoSpectatorClass)
	{
		return SpawnPlayerControllerCommon(InRemoteRole, FVector::ZeroVector, FRotator::ZeroRotator, UShooterGameData::Get().DemoSpectatorClass.LoadSynchronous());
	}
	return SpawnPlayerControllerCommon(InRemoteRole, FVector::ZeroVector, FRotator::ZeroRotator, UShooterGameData::Get().PlayerControllerClass.LoadSynchronous());
}

UClass* AShooterGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController->IsA<AShooterAIController>())
	{
		return UShooterGameData::Get().BotPawnClass.LoadSynchronous(); // @TODO: Load ASync
	}
	return UShooterGameData::Get().DefaultPawnClass.LoadSynchronous(); // @TODO: Load ASync
}

void AShooterGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	if (AShooterPlayerController* PC = Cast<AShooterPlayerController>(NewPlayer))
	{
		// Since initial weapon is equipped before the pawn is added to the replication graph, need to resend the notify so that it can be added as a dependent actor
		AShooterCharacter* Character = Cast<AShooterCharacter>(PC->GetCharacter());
		if (Character)
		{
			AShooterCharacter::NotifyEquipWeapon.Broadcast(Character, Character->GetWeapon());
		}
		
		PC->ClientGameStarted();
	}
}

AActor* AShooterGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PreferredSpawns;
	TArray<APlayerStart*> FallbackSpawns;

	APlayerStart* BestStart = nullptr;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* TestSpawn = *It;
		if (TestSpawn->IsA<APlayerStartPIE>())
		{
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			BestStart = TestSpawn;
			break;
		}
		else
		{
			if (IsSpawnpointAllowed(TestSpawn, Player))
			{
				if (IsSpawnpointPreferred(TestSpawn, Player))
				{
					PreferredSpawns.Add(TestSpawn);
				}
				else
				{
					FallbackSpawns.Add(TestSpawn);
				}
			}
		}
	}

	if (!BestStart)
	{
		if (PreferredSpawns.Num() > 0)
		{
			BestStart = PreferredSpawns[FMath::RandHelper(PreferredSpawns.Num())];
		}
		else if (FallbackSpawns.Num() > 0)
		{
			BestStart = FallbackSpawns[FMath::RandHelper(FallbackSpawns.Num())];
		}
	}
	return BestStart ? BestStart : Super::ChoosePlayerStart_Implementation(Player);
}

bool AShooterGameMode::IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Player) const
{
	if (AShooterTeamStart* ShooterSpawnPoint = Cast<AShooterTeamStart>(SpawnPoint))
	{
		AShooterAIController* AIController = Cast<AShooterAIController>(Player);
		if (ShooterSpawnPoint->bNotForBots && AIController)
		{
			return false;
		}

		if (ShooterSpawnPoint->bNotForPlayers && !AIController)
		{
			return false;
		}
		return true;
	}

	return false;
}

bool AShooterGameMode::IsSpawnpointPreferred(APlayerStart* SpawnPoint, AController* Player) const
{
	AShooterCharacter* CharacterCDO = UShooterGameData::Get().DefaultPawnClass.LoadSynchronous()->GetDefaultObject<AShooterCharacter>();

	if(Player->IsA(AShooterAIController::StaticClass()))
	{
		CharacterCDO = UShooterGameData::Get().BotPawnClass.LoadSynchronous()->GetDefaultObject<AShooterCharacter>();
	}
	
	const FVector SpawnLocation = SpawnPoint->GetActorLocation();
	for (ACharacter* OtherPawn : TActorRange<ACharacter>(GetWorld()))
	{
		if (OtherPawn != CharacterCDO)
		{
			const float CombinedHeight = (CharacterCDO->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + OtherPawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) * 2.0f;
			const float CombinedRadius = CharacterCDO->GetCapsuleComponent()->GetScaledCapsuleRadius() + OtherPawn->GetCapsuleComponent()->GetScaledCapsuleRadius();
			const FVector OtherLocation = OtherPawn->GetActorLocation();
	
			// Check if player start overlaps this pawn
			if (FMath::Abs(SpawnLocation.Z - OtherLocation.Z) < CombinedHeight && (SpawnLocation - OtherLocation).Size2D() < CombinedRadius)
			{
				return false;
			}
		}
	}
	return true;
}

void AShooterGameMode::CreateBotControllers()
{
	int32 ExistingBots = 0;

	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{		
		if ((*It)->IsA(AShooterAIController::StaticClass())) // Controller belongs to a bot player.
		{
			++ExistingBots;
		}
	}

	// Create any necessary AIControllers.  Hold off on Pawn creation until pawns are actually necessary or need recreating.	
	int32 BotNum = ExistingBots;
	for (int32 Index = 0; Index < MaxBots - ExistingBots; ++Index)
	{
		CreateBot(BotNum + Index);
	}
}

AShooterAIController* AShooterGameMode::CreateBot(int32 BotNum)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = nullptr;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.OverrideLevel = nullptr;

	AShooterAIController* AIController = GetWorld()->SpawnActor<AShooterAIController>(SpawnInfo);
	InitBot(AIController, BotNum);

	return AIController;
}

void AShooterGameMode::StartBots()
{
	// Checking number of existing human player.
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{		
		if (AShooterAIController* AIC = Cast<AShooterAIController>(*It))
		{
			RestartPlayer(AIC);
		}
	}	
}

void AShooterGameMode::InitBot(AShooterAIController* AIController, int32 BotNum)
{	
	if (AIController && AIController->PlayerState)
	{
		FString BotName = FString::Printf(TEXT("Bot %d"), BotNum);
		AIController->PlayerState->SetPlayerName(BotName);
	}		
}

void AShooterGameMode::RestartGame()
{
	// Hide the scoreboard too.
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		if (AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(*It))
		{
			if (AShooterHUD* ShooterHUD = Cast<AShooterHUD>(PlayerController->GetHUD()))
			{
				// Passing true to bFocus here ensures that focus is returned to the game viewport.
				ShooterHUD->ShowScoreboard(false, true);
			}
		}
	}
	Super::RestartGame();
}


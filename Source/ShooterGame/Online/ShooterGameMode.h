// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ShooterGameMode.generated.h"

class APlayerStart;
class AShooterAIController;
class AShooterPlayerController;
class AShooterPlayerState;
class AShooterPickup;
class FUniqueNetId;

UCLASS(config=Game)
class SHOOTERGAME_API AShooterGameMode : public AGameMode
{
	GENERATED_BODY()
public:

	AShooterGameMode();

	UFUNCTION(exec)
	void SetAllowBots(bool bInAllowBots, int32 InMaxBots = 8);

	virtual void PreInitializeComponents() override;
	virtual void InitGameState() override;

	/** Initialize the game. This is called before actors' PreInitializeComponents. */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/** Accept or reject a player attempting to join the server. Fails login if you set the ErrorMessage to a non-empty string. */
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	/** starts match warmup */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void InitializeHUDForPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual TSubclassOf<APlayerController> GetPlayerControllerClassToSpawnForSeamlessTravel(APlayerController* PreviousPC) override;
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, const FString& Options) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	/** prevents friendly fire */
	virtual float ModifyDamage(float Damage, AActor* DamagedActor, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const;

	virtual void Killed(AController* Killer, AController* KilledPlayer, APawn* KilledPawn, const UDamageType* DamageType);

	/** can players damage each other? */
	virtual bool CanDealDamage(AShooterPlayerState* DamageInstigator, AShooterPlayerState* DamagedPlayer) const;
	
	virtual bool AllowCheats(APlayerController* P) override;

	/** update remaining time */
	virtual void 			DefaultTimer();

	virtual void			HandleMatchIsWaitingToStart() override;
	virtual void 			HandleMatchHasStarted() override;
	virtual void 			HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void 			RestartGame() override;
	void 					CreateBotControllers();
	AShooterAIController*	CreateBot(int32 BotNum);	

	virtual void PostInitProperties() override;

protected:

	/** delay between first player login and starting match */
	UPROPERTY(config)
	int32 WarmupTime;

	/** match duration */
	UPROPERTY(config)
	int32 RoundTime;

	UPROPERTY(config)
	int32 TimeBetweenMatches;

	/** score for kill */
	UPROPERTY(config)
	int32 KillScore;

	/** score for death */
	UPROPERTY(config)
	int32 DeathScore;

	/** scale for self instigated damage */
	UPROPERTY(config)
	float DamageSelfScale;

	UPROPERTY(config)
	int32 MaxBots;

	UPROPERTY()
	TArray<AShooterAIController*> BotControllers;

	UPROPERTY(config)
	TSubclassOf<AShooterPlayerController> PlatformPlayerControllerClass;
	
	/** Handle for efficient management of DefaultTimer timer */
	FTimerHandle TimerHandle_DefaultTimer;

	bool bNeedsBotCreation;

	bool bAllowBots;		

	/** spawning all bots for this game */
	void StartBots();

	/** initialization for bot after creation */
	virtual void InitBot(AShooterAIController* AIC, int32 BotNum);

	virtual void DetermineMatchWinner();
	virtual bool IsWinner(AShooterPlayerState* PlayerState) const;

	/** check if player can use spawnpoint */
	virtual bool IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Player) const;

	/** check if player should use spawnpoint */
	virtual bool IsSpawnpointPreferred(APlayerStart* SpawnPoint, AController* Player) const;

	virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;	

public:	

	UFUNCTION(exec)
	void FinishMatch();

	/*Finishes the match and bumps everyone to main menu.*/
	/*Only GameInstance should call this function */
	void RequestFinishAndExitToMainMenu();

	/** get the name of the bots count option used in server travel URL */
	static FString GetBotsCountOptionName();

	UPROPERTY()
	TArray<AShooterPickup*> LevelPickups;

};

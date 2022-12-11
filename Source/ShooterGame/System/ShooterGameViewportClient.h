// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "ShooterGameViewportClient.generated.h"

UCLASS(Within=Engine, transient, config=Engine)
class UShooterGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
public:

	UShooterGameViewportClient();

 	// start UGameViewportClient interface
 	void NotifyPlayerAdded( int32 PlayerIndex, ULocalPlayer* AddedPlayer ) override;

	void ShowLoadingScreen();
	void HideLoadingScreen();

	//FTicker Funcs
	virtual void Tick(float DeltaSeconds) override;	

	virtual	void BeginDestroy() override;
	virtual void DetachViewportClient() override;
	void ReleaseSlateResources();

#if WITH_EDITOR
	virtual void DrawTransition(class UCanvas* Canvas) override;
#endif //WITH_EDITOR
	// end UGameViewportClient interface

protected:
	void HideExistingWidgets();
	void ShowExistingWidgets();

	/** List of viewport content that the viewport is tracking */
	TArray<TSharedRef<class SWidget>>				ViewportContentStack;

	TArray<TSharedRef<class SWidget>>				HiddenViewportContentStack;

	TSharedPtr<class SWidget>						OldFocusWidget;
};
// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameViewportClient.h"
#include "Player/ShooterLocalPlayer.h"

#include "Engine.h"

UShooterGameViewportClient::UShooterGameViewportClient()
{
	SetSuppressTransitionMessage(true);
}

void UShooterGameViewportClient::NotifyPlayerAdded(int32 PlayerIndex, ULocalPlayer* AddedPlayer)
{
	Super::NotifyPlayerAdded(PlayerIndex, AddedPlayer);

 	UShooterLocalPlayer* const ShooterLP = Cast<UShooterLocalPlayer>(AddedPlayer);
 	if (ShooterLP)
 	{
 		ShooterLP->LoadPersistentUser();
 	}
}

void UShooterGameViewportClient::ShowExistingWidgets()
{
	// We shouldn't have any visible widgets at this point
	check( ViewportContentStack.Num() == 0 );

	// Unhide all of the previously hidden widgets
	for ( int32 i = 0; i < HiddenViewportContentStack.Num(); i++ )
	{
		AddViewportWidgetContent( HiddenViewportContentStack[i] );
	}

	check( ViewportContentStack.Num() == HiddenViewportContentStack.Num() );

	// Done with these
	HiddenViewportContentStack.Empty();
}

void UShooterGameViewportClient::ShowLoadingScreen()
{

}

void UShooterGameViewportClient::HideLoadingScreen()
{

}

void UShooterGameViewportClient::Tick(float DeltaSeconds)
{
	//if ( DialogWidget.IsValid() && !LoadingScreenWidget.IsValid() )
	//{
	//	// Make sure the dialog widget always has focus
	//	if ( FSlateApplication::Get().GetKeyboardFocusedWidget() != DialogWidget )
	//	{
	//		// Remember which widget had focus before we override it
	//		OldFocusWidget = FSlateApplication::Get().GetKeyboardFocusedWidget();
	//
	//		// Force focus back to dialog
	//		FSlateApplication::Get().SetKeyboardFocus( DialogWidget, EFocusCause::SetDirectly );
	//	}
	//}
}

#if WITH_EDITOR
void UShooterGameViewportClient::DrawTransition(UCanvas* Canvas)
{
	if (GetOuterUEngine() != NULL)
	{
		ETransitionType Type = GetOuterUEngine()->TransitionType;
		switch (Type)
		{
		case ETransitionType::Connecting:
			DrawTransitionMessage(Canvas, NSLOCTEXT("GameViewportClient", "ConnectingMessage", "CONNECTING").ToString());
			break;
		case ETransitionType::WaitingToConnect:
			DrawTransitionMessage(Canvas, NSLOCTEXT("GameViewportClient", "Waitingtoconnect", "Waiting to connect...").ToString());
			break;	
		}
	}
}
#endif //WITH_EDITOR

void UShooterGameViewportClient::BeginDestroy()
{
	ReleaseSlateResources();

	Super::BeginDestroy();
}

void UShooterGameViewportClient::DetachViewportClient()
{
	Super::DetachViewportClient();

	ReleaseSlateResources();
}

void UShooterGameViewportClient::ReleaseSlateResources()
{
	//OldFocusWidget.Reset();
	//LoadingScreenWidget.Reset();
	//ViewportContentStack.Empty();
	//HiddenViewportContentStack.Empty();
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIShootingPlayerController.h"

#include "AIShootingGameModeBase.h"
#include "AIShootingGameState.h"
#include "AIShootingPlayerPawn.h"
#include "Components/InputComponent.h"

void AAIShootingPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("MoveHorizontal"), this, &AAIShootingPlayerController::MoveHorizontal);
	InputComponent->BindAxis(TEXT("MoveVertical"), this, &AAIShootingPlayerController::MoveVertical);
	InputComponent->BindAction(TEXT("FireUp"), IE_Pressed, this, &AAIShootingPlayerController::FireUpPressed);
	InputComponent->BindAction(TEXT("FireUp"), IE_Released, this, &AAIShootingPlayerController::FireUpReleased);
	InputComponent->BindAction(TEXT("FireDown"), IE_Pressed, this, &AAIShootingPlayerController::FireDownPressed);
	InputComponent->BindAction(TEXT("FireDown"), IE_Released, this, &AAIShootingPlayerController::FireDownReleased);
	InputComponent->BindAction(TEXT("FireLeft"), IE_Pressed, this, &AAIShootingPlayerController::FireLeftPressed);
	InputComponent->BindAction(TEXT("FireLeft"), IE_Released, this, &AAIShootingPlayerController::FireLeftReleased);
	InputComponent->BindAction(TEXT("FireRight"), IE_Pressed, this, &AAIShootingPlayerController::FireRightPressed);
	InputComponent->BindAction(TEXT("FireRight"), IE_Released, this, &AAIShootingPlayerController::FireRightReleased);
	InputComponent->BindAction(TEXT("Restart"), IE_Pressed, this, &AAIShootingPlayerController::RestartPressed);
}

void AAIShootingPlayerController::FireUpPressed() { if (AAIShootingPlayerPawn* PlayerPawn = GetPawn<AAIShootingPlayerPawn>()) { PlayerPawn->SetFireUpPressed(true); } }
void AAIShootingPlayerController::FireUpReleased() { if (AAIShootingPlayerPawn* PlayerPawn = GetPawn<AAIShootingPlayerPawn>()) { PlayerPawn->SetFireUpPressed(false); } }
void AAIShootingPlayerController::FireDownPressed() { if (AAIShootingPlayerPawn* PlayerPawn = GetPawn<AAIShootingPlayerPawn>()) { PlayerPawn->SetFireDownPressed(true); } }
void AAIShootingPlayerController::FireDownReleased() { if (AAIShootingPlayerPawn* PlayerPawn = GetPawn<AAIShootingPlayerPawn>()) { PlayerPawn->SetFireDownPressed(false); } }
void AAIShootingPlayerController::FireLeftPressed() { if (AAIShootingPlayerPawn* PlayerPawn = GetPawn<AAIShootingPlayerPawn>()) { PlayerPawn->SetFireLeftPressed(true); } }
void AAIShootingPlayerController::FireLeftReleased() { if (AAIShootingPlayerPawn* PlayerPawn = GetPawn<AAIShootingPlayerPawn>()) { PlayerPawn->SetFireLeftPressed(false); } }
void AAIShootingPlayerController::FireRightPressed() { if (AAIShootingPlayerPawn* PlayerPawn = GetPawn<AAIShootingPlayerPawn>()) { PlayerPawn->SetFireRightPressed(true); } }
void AAIShootingPlayerController::FireRightReleased() { if (AAIShootingPlayerPawn* PlayerPawn = GetPawn<AAIShootingPlayerPawn>()) { PlayerPawn->SetFireRightPressed(false); } }

void AAIShootingPlayerController::RestartPressed()
{
	const AAIShootingGameState* AIShootingGameState = GetWorld() ? GetWorld()->GetGameState<AAIShootingGameState>() : nullptr;
	if (!AIShootingGameState || !AIShootingGameState->IsGameOver())
	{
		return;
	}

	if (AAIShootingGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AAIShootingGameModeBase>())
	{
		GameMode->RequestRestart();
	}
}

void AAIShootingPlayerController::MoveHorizontal(float Value)
{
	if (AAIShootingPlayerPawn* PlayerPawn = GetPawn<AAIShootingPlayerPawn>())
	{
		PlayerPawn->SetMoveHorizontalInput(Value);
	}
}

void AAIShootingPlayerController::MoveVertical(float Value)
{
	if (AAIShootingPlayerPawn* PlayerPawn = GetPawn<AAIShootingPlayerPawn>())
	{
		PlayerPawn->SetMoveVerticalInput(Value);
	}
}

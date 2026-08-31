// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIShootingGameModeBase.h"

#include "AIShootingGameState.h"
#include "AIShootingPlayerController.h"
#include "AIShootingPlayerPawn.h"

AAIShootingGameModeBase::AAIShootingGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
	GameStateClass = AAIShootingGameState::StaticClass();
	DefaultPawnClass = AAIShootingPlayerPawn::StaticClass();
	PlayerControllerClass = AAIShootingPlayerController::StaticClass();
}

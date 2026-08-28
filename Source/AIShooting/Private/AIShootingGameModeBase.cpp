// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIShootingGameModeBase.h"

#include "AIShootingGameState.h"

AAIShootingGameModeBase::AAIShootingGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
	GameStateClass = AAIShootingGameState::StaticClass();
}

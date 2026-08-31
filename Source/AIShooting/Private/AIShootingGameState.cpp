// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIShootingGameState.h"

AAIShootingGameState::AAIShootingGameState()
	: GamePhase(EAIShootingGamePhase::Playing)
	, KillCount(0)
{
	PrimaryActorTick.bCanEverTick = false;
}

EAIShootingGamePhase AAIShootingGameState::GetGamePhase() const
{
	return GamePhase;
}

bool AAIShootingGameState::IsPlaying() const
{
	return GamePhase == EAIShootingGamePhase::Playing;
}

bool AAIShootingGameState::IsGameOver() const
{
	return GamePhase == EAIShootingGamePhase::GameOver;
}

int32 AAIShootingGameState::GetKillCount() const
{
	return KillCount;
}

void AAIShootingGameState::SetGamePhase(const EAIShootingGamePhase NewGamePhase)
{
	GamePhase = NewGamePhase;
}

bool AAIShootingGameState::TryEnterGameOver()
{
	if (!IsPlaying())
	{
		return false;
	}

	GamePhase = EAIShootingGamePhase::GameOver;
	return true;
}

void AAIShootingGameState::AddKill()
{
	++KillCount;
}

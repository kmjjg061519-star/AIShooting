// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "AIShootingGameState.generated.h"

UENUM(BlueprintType)
enum class EAIShootingGamePhase : uint8
{
	Playing,
	GameOver
};

UCLASS()
class AISHOOTING_API AAIShootingGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AAIShootingGameState();

	EAIShootingGamePhase GetGamePhase() const;
	bool IsPlaying() const;
	bool IsGameOver() const;
	int32 GetKillCount() const;

	void SetGamePhase(EAIShootingGamePhase NewGamePhase);
	/** Transitions from Playing to GameOver exactly once. */
	bool TryEnterGameOver();
	void AddKill();

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Game State")
	EAIShootingGamePhase GamePhase;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Game State", meta = (ClampMin = "0"))
	int32 KillCount;
};

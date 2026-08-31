// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AIShootingGameModeBase.generated.h"

UCLASS()
class AISHOOTING_API AAIShootingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAIShootingGameModeBase();

	virtual void StartPlay() override;

	/** Called by the player pawn after its health reaches zero. */
	void NotifyPlayerDied();

	/** Reloads the current level only while the game is over. */
	void RequestRestart();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<class AAIShootingEnemyPawn> EnemyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning", meta = (ClampMin = "0.01", Units = "s"))
	float EnemySpawnIntervalSeconds = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning", meta = (ClampMin = "0.0", Units = "cm"))
	float EnemySpawnMarginUnits = 100.0f;

protected:
	void SpawnEnemy();
	void ClearRemainingCombatActors();

private:
	FTimerHandle EnemySpawnTimerHandle;

	UPROPERTY()
	TObjectPtr<class AAIShootingCameraActor> GameplayCamera;

	UPROPERTY()
	TObjectPtr<class AAIShootingPlayerPawn> PlayerPawn;
};

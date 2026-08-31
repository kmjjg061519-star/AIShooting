// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIShootingGameModeBase.h"

#include "AIShootingCameraActor.h"
#include "AIShootingEnemyPawn.h"
#include "AIShootingGameState.h"
#include "AIShootingLog.h"
#include "AIShootingPlayerController.h"
#include "AIShootingPlayerPawn.h"
#include "AIShootingPlayerProjectile.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AAIShootingGameModeBase::AAIShootingGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
	GameStateClass = AAIShootingGameState::StaticClass();
	DefaultPawnClass = AAIShootingPlayerPawn::StaticClass();
	PlayerControllerClass = AAIShootingPlayerController::StaticClass();
}

void AAIShootingGameModeBase::StartPlay()
{
	Super::StartPlay();

	for (TActorIterator<AAIShootingCameraActor> CameraIterator(GetWorld()); CameraIterator; ++CameraIterator)
	{
		GameplayCamera = *CameraIterator;
		break;
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerPawn = Cast<AAIShootingPlayerPawn>(PlayerController->GetPawn());
	}
	if (!PlayerPawn)
	{
		UE_LOG(LogAIShooting, Warning, TEXT("Enemy spawning is waiting for a valid AIShooting player pawn."));
	}
	if (!GameplayCamera)
	{
		UE_LOG(LogAIShooting, Warning, TEXT("Enemy spawning is waiting for an AIShooting camera actor."));
	}
	if (!EnemyClass)
	{
		UE_LOG(LogAIShooting, Warning, TEXT("Enemy spawning is disabled because EnemyClass is not configured."));
	}

	GetWorldTimerManager().SetTimer(EnemySpawnTimerHandle, this, &AAIShootingGameModeBase::SpawnEnemy,
		EnemySpawnIntervalSeconds, true, EnemySpawnIntervalSeconds);
}

void AAIShootingGameModeBase::SpawnEnemy()
{
	const AAIShootingGameState* AIShootingGameState = GetGameState<AAIShootingGameState>();
	if (!AIShootingGameState || !AIShootingGameState->IsPlaying())
	{
		return;
	}

	if (!EnemyClass)
	{
		UE_LOG(LogAIShooting, Warning, TEXT("Skipped enemy spawn because EnemyClass is not configured."));
		return;
	}

	if (!GameplayCamera)
	{
		for (TActorIterator<AAIShootingCameraActor> CameraIterator(GetWorld()); CameraIterator; ++CameraIterator)
		{
			GameplayCamera = *CameraIterator;
			break;
		}
	}
	if (!PlayerPawn)
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			PlayerPawn = Cast<AAIShootingPlayerPawn>(PlayerController->GetPawn());
		}
	}
	if (!GameplayCamera || !PlayerPawn)
	{
		UE_LOG(LogAIShooting, Warning, TEXT("Skipped enemy spawn because the gameplay camera or player pawn is unavailable."));
		return;
	}

	const FVector2D HalfExtents = GameplayCamera->GetVisibleHalfExtents();
	const float YLimit = HalfExtents.X - EnemySpawnMarginUnits;
	if (YLimit <= 0.0f || HalfExtents.Y <= EnemySpawnMarginUnits)
	{
		UE_LOG(LogAIShooting, Warning, TEXT("Skipped enemy spawn because EnemySpawnMarginUnits does not fit inside the camera bounds."));
		return;
	}

	const FVector CameraLocation = GameplayCamera->GetActorLocation();
	const FVector PlayerLocation = PlayerPawn->GetActorLocation();
	const FVector SpawnLocation(
		PlayerLocation.X,
		CameraLocation.Y + FMath::FRandRange(-YLimit, YLimit),
		CameraLocation.Z + HalfExtents.Y - EnemySpawnMarginUnits);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AAIShootingEnemyPawn>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
}

void AAIShootingGameModeBase::NotifyPlayerDied()
{
	AAIShootingGameState* AIShootingGameState = GetGameState<AAIShootingGameState>();
	if (!AIShootingGameState || !AIShootingGameState->TryEnterGameOver())
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(EnemySpawnTimerHandle);
	ClearRemainingCombatActors();

	UE_LOG(LogAIShooting, Log, TEXT("Game over. Press R to restart."));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, TEXT("GAME OVER - Press R to restart"));
	}
}

void AAIShootingGameModeBase::RequestRestart()
{
	const AAIShootingGameState* AIShootingGameState = GetGameState<AAIShootingGameState>();
	if (!AIShootingGameState || !AIShootingGameState->IsGameOver())
	{
		return;
	}

	const FName CurrentLevelName(*GetWorld()->GetName());
	UE_LOG(LogAIShooting, Log, TEXT("Restarting level %s."), *CurrentLevelName.ToString());
	UGameplayStatics::OpenLevel(this, CurrentLevelName, true);
}

void AAIShootingGameModeBase::ClearRemainingCombatActors()
{
	for (TActorIterator<AAIShootingEnemyPawn> EnemyIterator(GetWorld()); EnemyIterator; ++EnemyIterator)
	{
		EnemyIterator->Destroy();
	}

	for (TActorIterator<AAIShootingPlayerProjectile> ProjectileIterator(GetWorld()); ProjectileIterator; ++ProjectileIterator)
	{
		ProjectileIterator->Destroy();
	}
}

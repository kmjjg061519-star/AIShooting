// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIShootingEnemyAIController.h"

#include "AIShootingEnemyPawn.h"
#include "AIShootingGameState.h"
#include "Kismet/GameplayStatics.h"

AAIShootingEnemyAIController::AAIShootingEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAIShootingEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const AAIShootingGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AAIShootingGameState>() : nullptr;
	if (!GameState || !GameState->IsPlaying())
	{
		return;
	}

	AAIShootingEnemyPawn* EnemyPawn = GetPawn<AAIShootingEnemyPawn>();
	APawn* PlayerPawn = GetTargetPlayerPawn();
	if (!EnemyPawn || !PlayerPawn)
	{
		return;
	}

	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - EnemyPawn->GetActorLocation();
	DirectionToPlayer.X = 0.0f;
	EnemyPawn->MoveInPlane(DirectionToPlayer.GetSafeNormal(), DeltaTime);
}

APawn* AAIShootingEnemyAIController::GetTargetPlayerPawn()
{
	if (!IsValid(TargetPlayerPawn.Get()))
	{
		TargetPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	}

	return TargetPlayerPawn.Get();
}

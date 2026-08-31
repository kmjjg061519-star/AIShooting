// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIShootingEnemyAIController.generated.h"

class APawn;

/** Direct YZ-plane player tracking without navigation or behavior-tree dependencies. */
UCLASS()
class AISHOOTING_API AAIShootingEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAIShootingEnemyAIController();

	virtual void Tick(float DeltaTime) override;

private:
	APawn* GetTargetPlayerPawn();

	TWeakObjectPtr<APawn> TargetPlayerPawn;
};

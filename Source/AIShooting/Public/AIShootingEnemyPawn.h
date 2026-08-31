// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIShootingEnemyPawn.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;

/** Simple enemy pawn that is permanently constrained to the world YZ gameplay plane. */
UCLASS()
class AISHOOTING_API AAIShootingEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	AAIShootingEnemyPawn();

	virtual void BeginPlay() override;

	/** Moves in a normalized YZ direction while preserving the plane captured at spawn. */
	void MoveInPlane(const FVector& Direction, float DeltaTime);
	void Die();
	bool IsAlive() const { return bIsAlive; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Components")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Components")
	TObjectPtr<UStaticMeshComponent> VisualComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Movement", meta = (ClampMin = "0.0", Units = "cm/s"))
	float MoveSpeedUnitsPerSecond = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Combat")
	bool bIsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Combat")
	bool bHasHitTarget = false;

private:
	UFUNCTION()
	void HandleCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	float FixedWorldX = 0.0f;
};

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIShootingPlayerPawn.generated.h"

class AAIShootingCameraActor;
class AAIShootingPlayerProjectile;
class USceneComponent;
class USphereComponent;
class UStaticMeshComponent;

/** Player pawn constrained to the project's fixed world YZ gameplay plane. */
UCLASS()
class AISHOOTING_API AAIShootingPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AAIShootingPlayerPawn();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetMoveHorizontalInput(float Value);
	void SetMoveVerticalInput(float Value);
	void SetFireUpPressed(bool bPressed);
	void SetFireDownPressed(bool bPressed);
	void SetFireLeftPressed(bool bPressed);
	void SetFireRightPressed(bool bPressed);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Components")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Components")
	TObjectPtr<UStaticMeshComponent> VisualComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Components")
	TObjectPtr<USceneComponent> ProjectileSpawnOrigin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Movement", meta = (ClampMin = "0.0", Units = "cm/s"))
	float MoveSpeedUnitsPerSecond = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Fire")
	TSubclassOf<AAIShootingPlayerProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Fire", meta = (ClampMin = "0.01", Units = "s"))
	float FireIntervalSeconds = 0.2f;

protected:
	void MoveWithinGameplayBounds(float DeltaTime);
	void UpdateFireState();
	FVector GetFireDirection() const;
	void FireProjectile();

private:
	float FixedWorldX = 0.0f;
	float MoveHorizontalInput = 0.0f;
	float MoveVerticalInput = 0.0f;
	bool bFireUpPressed = false;
	bool bFireDownPressed = false;
	bool bFireLeftPressed = false;
	bool bFireRightPressed = false;

	FTimerHandle FireTimerHandle;

	UPROPERTY()
	TObjectPtr<AAIShootingCameraActor> GameplayCamera;
};

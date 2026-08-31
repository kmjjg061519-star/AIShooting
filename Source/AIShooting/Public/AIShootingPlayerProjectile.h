// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIShootingPlayerProjectile.generated.h"

class AAIShootingCameraActor;
class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;

/** A player-fired projectile which travels only on the fixed YZ gameplay plane. */
UCLASS()
class AISHOOTING_API AAIShootingPlayerProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAIShootingPlayerProjectile();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Must be called once while the actor is deferred, before its first tick. */
	void InitializeProjectile(const FVector& InDirection, float InFixedWorldX);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Components")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Components")
	TObjectPtr<UStaticMeshComponent> VisualComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Movement", meta = (ClampMin = "0.0", Units = "cm/s"))
	float MoveSpeedUnitsPerSecond = 1800.0f;

private:
	bool IsOutsideCameraBounds() const;

	UFUNCTION()
	void HandleCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	float FixedWorldX = 0.0f;
	bool bHasInitializedDirection = false;
	bool bHasHitTarget = false;

	UPROPERTY()
	TObjectPtr<AAIShootingCameraActor> GameplayCamera;
};

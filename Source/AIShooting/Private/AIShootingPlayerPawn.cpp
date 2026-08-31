// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIShootingPlayerPawn.h"

#include "AIShootingCameraActor.h"
#include "AIShootingPlayerProjectile.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

AAIShootingPlayerPawn::AAIShootingPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SetRootComponent(CollisionComponent);

	VisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualComponent"));
	VisualComponent->SetupAttachment(CollisionComponent);
	VisualComponent->SetRelativeScale3D(FVector(0.7f, 0.7f, 1.0f));
	VisualComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMesh(TEXT("/Engine/BasicShapes/Cone.Cone"));
	if (ConeMesh.Succeeded())
	{
		VisualComponent->SetStaticMesh(ConeMesh.Object);
	}

	// The basic cone's local +Z axis points toward its tip, matching screen-up/world +Z.
	ProjectileSpawnOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnOrigin"));
	ProjectileSpawnOrigin->SetupAttachment(CollisionComponent);
	ProjectileClass = AAIShootingPlayerProjectile::StaticClass();
}

void AAIShootingPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	FixedWorldX = GetActorLocation().X;
	for (TActorIterator<AAIShootingCameraActor> CameraIterator(GetWorld()); CameraIterator; ++CameraIterator)
	{
		GameplayCamera = *CameraIterator;
		break;
	}
}

void AAIShootingPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveWithinGameplayBounds(DeltaTime);
}

void AAIShootingPlayerPawn::SetMoveHorizontalInput(float Value)
{
	MoveHorizontalInput = Value;
}

void AAIShootingPlayerPawn::SetMoveVerticalInput(float Value)
{
	MoveVerticalInput = Value;
}

void AAIShootingPlayerPawn::SetFireUpPressed(bool bPressed) { bFireUpPressed = bPressed; UpdateFireState(); }
void AAIShootingPlayerPawn::SetFireDownPressed(bool bPressed) { bFireDownPressed = bPressed; UpdateFireState(); }
void AAIShootingPlayerPawn::SetFireLeftPressed(bool bPressed) { bFireLeftPressed = bPressed; UpdateFireState(); }
void AAIShootingPlayerPawn::SetFireRightPressed(bool bPressed) { bFireRightPressed = bPressed; UpdateFireState(); }

void AAIShootingPlayerPawn::MoveWithinGameplayBounds(float DeltaTime)
{
	const FVector2D InputVector(MoveHorizontalInput, MoveVerticalInput);
	const FVector2D ClampedInput = InputVector.GetClampedToMaxSize(1.0f);
	const FVector MovementDelta(0.0f, ClampedInput.X * MoveSpeedUnitsPerSecond * DeltaTime, ClampedInput.Y * MoveSpeedUnitsPerSecond * DeltaTime);
	FVector NewLocation = GetActorLocation() + MovementDelta;
	NewLocation.X = FixedWorldX;

	if (GameplayCamera)
	{
		NewLocation = GameplayCamera->ClampLocationToVisibleBounds(NewLocation, CollisionComponent->GetScaledSphereRadius());
		NewLocation.X = FixedWorldX;
	}

	SetActorLocation(NewLocation, false);
}

void AAIShootingPlayerPawn::UpdateFireState()
{
	if (!GetFireDirection().IsNearlyZero())
	{
		if (!GetWorldTimerManager().IsTimerActive(FireTimerHandle))
		{
			FireProjectile();
			GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AAIShootingPlayerPawn::FireProjectile, FireIntervalSeconds, true);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
	}
}

FVector AAIShootingPlayerPawn::GetFireDirection() const
{
	const float Horizontal = (bFireRightPressed ? 1.0f : 0.0f) - (bFireLeftPressed ? 1.0f : 0.0f);
	const float Vertical = (bFireUpPressed ? 1.0f : 0.0f) - (bFireDownPressed ? 1.0f : 0.0f);
	return FVector(0.0f, Horizontal, Vertical).GetSafeNormal();
}

void AAIShootingPlayerPawn::FireProjectile()
{
	const FVector Direction = GetFireDirection();
	if (!ProjectileClass || Direction.IsNearlyZero())
	{
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
		return;
	}

	FVector SpawnLocation = ProjectileSpawnOrigin->GetComponentLocation();
	SpawnLocation.X = FixedWorldX;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;
	if (AAIShootingPlayerProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAIShootingPlayerProjectile>(ProjectileClass, FTransform(SpawnLocation), this, this))
	{
		Projectile->InitializeProjectile(Direction, FixedWorldX);
		Projectile->FinishSpawning(FTransform(SpawnLocation));
	}
}

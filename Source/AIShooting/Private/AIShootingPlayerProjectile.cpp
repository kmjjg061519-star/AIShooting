// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIShootingPlayerProjectile.h"

#include "AIShootingCameraActor.h"
#include "AIShootingEnemyPawn.h"
#include "AIShootingGameState.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

AAIShootingPlayerProjectile::AAIShootingPlayerProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel2);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAIShootingPlayerProjectile::HandleCollisionOverlap);
	SetRootComponent(CollisionComponent);

	VisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualComponent"));
	VisualComponent->SetupAttachment(CollisionComponent);
	VisualComponent->SetRelativeScale3D(FVector(0.2f));
	VisualComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		VisualComponent->SetStaticMesh(CubeMesh.Object);
	}

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = MoveSpeedUnitsPerSecond;
	ProjectileMovement->MaxSpeed = MoveSpeedUnitsPerSecond;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = false;
}

void AAIShootingPlayerProjectile::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AAIShootingCameraActor> CameraIterator(GetWorld()); CameraIterator; ++CameraIterator)
	{
		GameplayCamera = *CameraIterator;
		break;
	}
}

void AAIShootingPlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const AAIShootingGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AAIShootingGameState>() : nullptr;
	if (!GameState || !GameState->IsPlaying())
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ProjectileMovement->StopMovementImmediately();
		return;
	}

	FVector Location = GetActorLocation();
	Location.X = FixedWorldX;
	SetActorLocation(Location, false);

	if (IsOutsideCameraBounds())
	{
		Destroy();
	}
}

void AAIShootingPlayerProjectile::InitializeProjectile(const FVector& InDirection, float InFixedWorldX)
{
	if (bHasInitializedDirection)
	{
		return;
	}

	const FVector Direction(0.0f, InDirection.Y, InDirection.Z);
	if (Direction.IsNearlyZero())
	{
		return;
	}

	bHasInitializedDirection = true;
	FixedWorldX = InFixedWorldX;
	ProjectileMovement->Velocity = Direction.GetSafeNormal() * MoveSpeedUnitsPerSecond;
	ProjectileMovement->InitialSpeed = MoveSpeedUnitsPerSecond;
	ProjectileMovement->MaxSpeed = MoveSpeedUnitsPerSecond;
}

bool AAIShootingPlayerProjectile::IsOutsideCameraBounds() const
{
	if (!GameplayCamera)
	{
		return false;
	}

	const FVector2D HalfExtents = GameplayCamera->GetVisibleHalfExtents();
	const FVector Location = GetActorLocation();
	const float Radius = CollisionComponent->GetScaledSphereRadius();
	return FMath::Abs(Location.Y) > HalfExtents.X + Radius || FMath::Abs(Location.Z) > HalfExtents.Y + Radius;
}

void AAIShootingPlayerProjectile::HandleCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAIShootingGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AAIShootingGameState>() : nullptr;
	if (bHasHitTarget || !GameState || !GameState->IsPlaying())
	{
		return;
	}

	if (AAIShootingEnemyPawn* EnemyPawn = Cast<AAIShootingEnemyPawn>(OtherActor))
	{
		bHasHitTarget = true;
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ProjectileMovement->StopMovementImmediately();
		EnemyPawn->Die();
		GameState->AddKill();
		Destroy();
	}
}

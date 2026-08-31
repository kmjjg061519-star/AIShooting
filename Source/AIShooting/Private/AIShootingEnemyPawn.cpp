// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIShootingEnemyPawn.h"

#include "AIShootingEnemyAIController.h"
#include "AIShootingGameState.h"
#include "AIShootingPlayerPawn.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AAIShootingEnemyPawn::AAIShootingEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel3);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAIShootingEnemyPawn::HandleCollisionOverlap);
	SetRootComponent(CollisionComponent);

	VisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualComponent"));
	VisualComponent->SetupAttachment(CollisionComponent);
	VisualComponent->SetRelativeScale3D(FVector(0.8f));
	VisualComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		VisualComponent->SetStaticMesh(SphereMesh.Object);
	}

	AIControllerClass = AAIShootingEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAIShootingEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	FixedWorldX = GetActorLocation().X;
}

void AAIShootingEnemyPawn::MoveInPlane(const FVector& Direction, float DeltaTime)
{
	const AAIShootingGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AAIShootingGameState>() : nullptr;
	if (!bIsAlive || !GameState || !GameState->IsPlaying())
	{
		return;
	}
	FVector PlaneDirection = Direction;
	PlaneDirection.X = 0.0f;
	PlaneDirection = PlaneDirection.GetSafeNormal();
	if (PlaneDirection.IsNearlyZero() || DeltaTime <= 0.0f)
	{
		return;
	}

	FVector NewLocation = GetActorLocation() + PlaneDirection * MoveSpeedUnitsPerSecond * DeltaTime;
	NewLocation.X = FixedWorldX;
	SetActorLocation(NewLocation, false);
}

void AAIShootingEnemyPawn::Die()
{
	if (!bIsAlive)
	{
		return;
	}

	bIsAlive = false;
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorEnableCollision(false);
	Destroy();
}

void AAIShootingEnemyPawn::HandleCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const AAIShootingGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AAIShootingGameState>() : nullptr;
	if (!bIsAlive || bHasHitTarget || !GameState || !GameState->IsPlaying())
	{
		return;
	}

	if (AAIShootingPlayerPawn* PlayerPawn = Cast<AAIShootingPlayerPawn>(OtherActor))
	{
		bHasHitTarget = true;
		PlayerPawn->ApplyEnemyHit();
		Die();
	}
}

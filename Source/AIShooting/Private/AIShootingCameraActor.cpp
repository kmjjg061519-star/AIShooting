// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIShootingCameraActor.h"

#include "Camera/CameraComponent.h"

AAIShootingCameraActor::AAIShootingCameraActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetActorLocation(FVector(-1500.0f, 0.0f, 0.0f));
	SetActorRotation(FRotator::ZeroRotator);

	UCameraComponent* Camera = GetCameraComponent();
	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	Camera->AspectRatio = ReferenceAspectRatio;
	Camera->bConstrainAspectRatio = true;
	Camera->OrthoWidth = ReferenceOrthoWidthUnits;
}

FVector2D AAIShootingCameraActor::GetVisibleHalfExtents() const
{
	const UCameraComponent* Camera = GetCameraComponent();
	const float AspectRatio = FMath::Max(Camera->AspectRatio, KINDA_SMALL_NUMBER);
	const float HalfWidth = Camera->OrthoWidth * 0.5f;
	const float HalfHeight = HalfWidth / AspectRatio;
	return FVector2D(HalfWidth, HalfHeight);
}

FVector AAIShootingCameraActor::ClampLocationToVisibleBounds(const FVector& Location, float EdgePaddingUnits) const
{
	const FVector2D HalfExtents = GetVisibleHalfExtents();
	const float EdgePadding = FMath::Max(EdgePaddingUnits, 0.0f);
	const float YLimit = FMath::Max(HalfExtents.X - EdgePadding, 0.0f);
	const float ZLimit = FMath::Max(HalfExtents.Y - EdgePadding, 0.0f);

	return FVector(
		Location.X,
		FMath::Clamp(Location.Y, -YLimit, YLimit),
		FMath::Clamp(Location.Z, -ZLimit, ZLimit));
}

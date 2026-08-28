// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "AIShootingCameraActor.generated.h"

/**
 * Fixed orthographic camera for the portrait YZ gameplay plane.
 * World +X is the viewing direction; Y and Z map to screen horizontal and vertical.
 */
UCLASS()
class AISHOOTING_API AAIShootingCameraActor : public ACameraActor
{
	GENERATED_BODY()

public:
	AAIShootingCameraActor();

	/** Returns the horizontal (Y) and vertical (Z) half extents visible at the reference framing. */
	UFUNCTION(BlueprintPure, Category = "Camera|Framing")
	FVector2D GetVisibleHalfExtents() const;

	/** Keeps a location inside the visible YZ area without changing its fixed X plane. */
	UFUNCTION(BlueprintPure, Category = "Camera|Framing")
	FVector ClampLocationToVisibleBounds(const FVector& Location, float EdgePaddingUnits = 0.0f) const;

	static constexpr int32 ReferenceViewportWidthPixels = 1024;
	static constexpr int32 ReferenceViewportHeightPixels = 2048;
	static constexpr float ReferenceAspectRatio = 0.5f;
	static constexpr float WorldUnitsPerReferencePixel = 2.0f;
	static constexpr float ReferenceOrthoWidthUnits = ReferenceViewportWidthPixels * WorldUnitsPerReferencePixel;
};

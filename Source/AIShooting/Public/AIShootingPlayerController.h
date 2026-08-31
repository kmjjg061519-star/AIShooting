// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AIShootingPlayerController.generated.h"

/** Legacy-input controller that forwards portrait-screen movement to the player pawn. */
UCLASS()
class AISHOOTING_API AAIShootingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

private:
	void MoveHorizontal(float Value);
	void MoveVertical(float Value);
	void FireUpPressed();
	void FireUpReleased();
	void FireDownPressed();
	void FireDownReleased();
	void FireLeftPressed();
	void FireLeftReleased();
	void FireRightPressed();
	void FireRightReleased();
};

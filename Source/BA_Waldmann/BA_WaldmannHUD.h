// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BA_WaldmannHUD.generated.h"

UCLASS()
class ABA_WaldmannHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABA_WaldmannHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};


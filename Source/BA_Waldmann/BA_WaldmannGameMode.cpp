// Copyright Epic Games, Inc. All Rights Reserved.

#include "BA_WaldmannGameMode.h"
#include "BA_WaldmannHUD.h"
#include "BA_WaldmannCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABA_WaldmannGameMode::ABA_WaldmannGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ABA_WaldmannHUD::StaticClass();
}

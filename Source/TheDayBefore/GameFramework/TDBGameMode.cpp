// Copyright TheDayBefore Project. All Rights Reserved.

#include "GameFramework/TDBGameMode.h"
#include "Characters/TDBCharacter.h"
#include "TDBPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"

ATDBGameMode::ATDBGameMode()
{
	// Set default pawn class to our FPS character
	DefaultPawnClass = ATDBCharacter::StaticClass();

	// Use our custom player controller
	PlayerControllerClass = ATDBPlayerController::StaticClass();

	SetupGameSettings();
}

void ATDBGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("TheDayBefore GameMode Started"));
}

void ATDBGameMode::SetupGameSettings()
{
	// Game settings can be configured here
	// For example: starting resources, game rules, etc.
}

void ATDBGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Log, TEXT("Player logged in: %s"), *NewPlayer->GetName());

	// Initialize player data
	// TODO: Load player save data, spawn in correct location
}

void ATDBGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	UE_LOG(LogTemp, Log, TEXT("Player logged out: %s"), *Exiting->GetName());

	// Save player data
	// TODO: Save player state before disconnect
}

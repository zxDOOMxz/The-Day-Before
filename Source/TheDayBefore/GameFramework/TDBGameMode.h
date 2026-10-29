// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TDBGameMode.generated.h"

/**
 * Base GameMode for The Day Before
 * Handles game rules, player spawning, and game state management
 */
UCLASS()
class ATDBGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATDBGameMode();

	// Default pawn class for players (FPS character)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TSubclassOf<APawn> DefaultPlayerPawn;

	// Spawn point for new players
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	AActor* PlayerStartPoint;

protected:
	virtual void BeginPlay() override;

	// AGameModeBase interface
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

private:
	void SetupGameSettings();
};

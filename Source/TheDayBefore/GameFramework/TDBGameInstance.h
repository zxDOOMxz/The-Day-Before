// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TDBGameInstance.generated.h"

class UTDBDamageSystem;

/**
 * Custom GameInstance for The Day Before
 * Manages global subsystems and game state
 */
UCLASS()
class UTDBGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	// Get damage system
	UFUNCTION(BlueprintPure, Category = "Systems")
	UTDBDamageSystem* GetDamageSystem() const;

private:
	UPROPERTY()
	UTDBDamageSystem* DamageSystem;
};

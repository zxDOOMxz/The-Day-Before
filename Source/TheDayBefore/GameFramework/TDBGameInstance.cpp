// Copyright TheDayBefore Project. All Rights Reserved.

#include "GameFramework/TDBGameInstance.h"
#include "Combat/TDBDamageSystem.h"

void UTDBGameInstance::Init()
{
	Super::Init();

	// Damage system is auto-created as a GameInstanceSubsystem
	DamageSystem = GetSubsystem<UTDBDamageSystem>();

	UE_LOG(LogTemp, Log, TEXT("TheDayBefore GameInstance Initialized"));
}

void UTDBGameInstance::Shutdown()
{
	DamageSystem = nullptr;

	Super::Shutdown();
}

UTDBDamageSystem* UTDBGameInstance::GetDamageSystem() const
{
	return DamageSystem;
}

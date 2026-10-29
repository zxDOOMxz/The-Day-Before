// Copyright TheDayBefore Project. All Rights Reserved.

#include "GameFramework/TDBGameInstance.h"
#include "Combat/TDBDamageSystem.h"

void UTDBGameInstance::Init()
{
	Super::Init();

	// Initialize damage system
	DamageSystem = NewObject<UTDBDamageSystem>(this, UTDBDamageSystem::StaticClass());
	DamageSystem->Initialize(FSubsystemCollectionBase());

	UE_LOG(LogTemp, Log, TEXT("TheDayBefore GameInstance Initialized"));
}

void UTDBGameInstance::Shutdown()
{
	if (DamageSystem)
	{
		DamageSystem->Deinitialize();
	}

	Super::Shutdown();
}

UTDBDamageSystem* UTDBGameInstance::GetDamageSystem() const
{
	return DamageSystem;
}

// Copyright TheDayBefore Project. All Rights Reserved.

#include "TheDayBefore.h"
#include "Modules/ModuleManager.h"

void FTheDayBeforeModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("TheDayBefore Module Started"));
}

void FTheDayBeforeModule::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("TheDayBefore Module Shutdown"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FTheDayBeforeModule, TheDayBefore, "TheDayBefore");

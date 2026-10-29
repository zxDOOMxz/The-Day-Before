// Copyright TheDayBefore Project. All Rights Reserved.

#include "TheDayBeforeServer.h"
#include "Modules/ModuleManager.h"

void FTheDayBeforeServerModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("TheDayBefore Server Module Started"));
}

void FTheDayBeforeServerModule::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("TheDayBefore Server Module Shutdown"));
}

IMPLEMENT_MODULE(FTheDayBeforeServerModule, TheDayBeforeServer);

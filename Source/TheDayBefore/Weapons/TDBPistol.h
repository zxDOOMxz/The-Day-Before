// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TDBWeaponBase.h"
#include "TDBPistol.generated.h"

/**
 * Pistol weapon class - example implementation
 */
UCLASS(Blueprintable)
class ATDBPistol : public ATDBWeaponBase
{
	GENERATED_BODY()

public:
	ATDBPistol();

protected:
	virtual void BeginPlay() override;
};

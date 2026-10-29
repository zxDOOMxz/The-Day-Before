// Copyright TheDayBefore Project. All Rights Reserved.

#include "Weapons/TDBPistol.h"

ATDBPistol::ATDBPistol()
{
	// Pistol-specific stats
	BaseDamage = 35.0f;
	FireRate = 400.0f; // Slower than rifle
	ReloadTime = 1.8f; // Faster reload
	MagazineSize = 15; // Smaller magazine
	CurrentAmmo = 15;
	Accuracy = 0.85f; // Less accurate than rifle
	RecoilVertical = 1.5f; // More recoil per shot
	RecoilHorizontal = 0.3f;
	BulletPenetration = 30.0f; // Lower penetration
	MaxRange = 30000.0f; // 300 meters effective range

	// Semi-automatic
	bIsAutomatic = false;
	bIsBoltAction = false;
}

void ATDBPistol::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("Pistol initialized"));
}

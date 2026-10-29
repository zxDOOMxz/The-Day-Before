// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDBWeaponBase.generated.h"

class USkeletalMeshComponent;
class UAudioComponent;
class UNiagaraComponent;

/**
 * Base class for all weapons in the game
 */
UCLASS(Blueprintable)
class ATDBWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ATDBWeaponBase();

	// Weapon mesh
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	// Muzzle flash effect
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UNiagaraComponent* MuzzleFlash;

	// Audio component for weapon sounds
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	UAudioComponent* WeaponAudio;

	// Weapon stats
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
	float BaseDamage = 25.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
	float FireRate = 600.0f; // Rounds per minute

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
	float ReloadTime = 2.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
	int32 MagazineSize = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
	int32 CurrentAmmo = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
	float Accuracy = 0.9f; // 0-1

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
	float RecoilVertical = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
	float RecoilHorizontal = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
	float BulletPenetration = 50.0f; // Armor penetration value

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
	float MaxRange = 50000.0f; // 500 meters

	// Weapon state
	UPROPERTY(ReplicatedUsing = OnRep_IsFiring, BlueprintReadOnly, Category = "Weapon|State")
	bool bIsFiring;

	UPROPERTY(ReplicatedUsing = OnReloading, BlueprintReadOnly, Category = "Weapon|State")
	bool bIsReloading;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon|State")
	int32 ReserveAmmo = 120;

	// Firing modes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Firing")
	bool bIsAutomatic = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Firing")
	bool bIsBoltAction = false;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void StartReload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void FinishReload();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool CanFire() const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool CanReload() const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	float GetAccuracy() const { return Accuracy; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetCurrentAmmo() const { return CurrentAmmo; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetMagazineSize() const { return MagazineSize; }

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Internal firing logic
	UFUNCTION()
	void FireWeapon();

	// Apply recoil
	UFUNCTION()
	void ApplyRecoil();

	// Play firing effects
	UFUNCTION()
	void PlayFireEffects();

	// RepNotify functions
	UFUNCTION()
	void OnRep_IsFiring();

	UFUNCTION()
	void OnReloading();

	// Timer handles
	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;

	// Time between shots
	float GetTimeBetweenShots() const { return 60.0f / FireRate; }
};

// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDBWeaponComponent.generated.h"

class ATDBWeaponBase;

/**
 * Weapon Component - manages weapon holding and switching
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UTDBWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTDBWeaponComponent();

	// Currently equipped weapon
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon, BlueprintReadOnly, Category = "Weapon")
	ATDBWeaponBase* CurrentWeapon;

	// Weapon socket name on character mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponSocket = FName("weapon_r");

	// Weapon classes available to equip
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TArray<TSubclassOf<ATDBWeaponBase>> AvailableWeapons;

	// Equipment functions
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(TSubclassOf<ATDBWeaponBase> WeaponClass);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UnequipWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool HasWeapon() const { return CurrentWeapon != nullptr; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	ATDBWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentWeapon();

private:
	void AttachWeaponToCharacter(ATDBWeaponBase* Weapon);
	void DetachWeaponFromCharacter(ATDBWeaponBase* Weapon);
};

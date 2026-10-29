// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"
#include "TDBDamageSystem.generated.h"

/**
 * Damage System - handles all damage calculations and application
 */
UCLASS()
class UTDBDamageSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Apply damage to an actor
	UFUNCTION(BlueprintCallable, Category = "Damage")
	float ApplyDamage(AActor* DamagedActor, float BaseDamage, AController* InstigatedBy, FVector HitLocation, FHitResult HitInfo);

	// Calculate armor penetration
	UFUNCTION(BlueprintPure, Category = "Damage")
	float CalculateArmorPenetration(float BulletPenetration, float ArmorValue);

	// Calculate final damage after armor
	UFUNCTION(BlueprintPure, Category = "Damage")
	float CalculateDamageWithArmor(float BaseDamage, float Penetration, float ArmorValue);

	// Apply damage to specific body part
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void ApplyLocalizedDamage(AActor* DamagedActor, float BaseDamage, FName BoneName, AController* InstigatedBy, FHitResult HitInfo);

	// Get damage multiplier for body part
	UFUNCTION(BlueprintPure, Category = "Damage")
	float GetBodyPartDamageMultiplier(FName BoneName);

private:
	// Body part damage multipliers
	TMap<FName, float> BodyPartMultipliers;

	void InitializeBodyPartMultipliers();
};

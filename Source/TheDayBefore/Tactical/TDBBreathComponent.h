// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDBBreathComponent.generated.h"

/**
 * Breath Component - handles breath holding while aiming (ADS)
 * Uses stamina system to limit breath holding duration
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UTDBBreathComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTDBBreathComponent();

	// Current breath stamina
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Breath")
	float BreathStamina;

	// Maximum breath stamina
	UPROPERTY(EditDefaultsOnly, Category = "Breath", meta = (ClampMin = "1.0"))
	float MaxBreathStamina = 100.0f;

	// Stamina drain rate per second while holding breath
	UPROPERTY(EditDefaultsOnly, Category = "Breath", meta = (ClampMin = "0.0"))
	float BreathDrainRate = 10.0f;

	// Stamina recovery rate per second
	UPROPERTY(EditDefaultsOnly, Category = "Breath", meta = (ClampMin = "0.0"))
	float BreathRecoveryRate = 15.0f;

	// Camera sway intensity (based on stamina)
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Breath")
	float BreathSwayIntensity;

	// Maximum camera sway when stamina is depleted
	UPROPERTY(EditDefaultsOnly, Category = "Breath")
	float MaxBreathSway = 2.0f;

	// Stamina threshold for sway to start
	UPROPERTY(EditDefaultsOnly, Category = "Breath")
	float SwayThreshold = 50.0f;

	// Breath control functions
	UFUNCTION(BlueprintCallable, Category = "Breath")
	void HoldBreath();

	UFUNCTION(BlueprintCallable, Category = "Breath")
	void ReleaseBreath();

	// Get breath stamina percentage (0-1)
	UFUNCTION(BlueprintPure, Category = "Breath")
	float GetBreathStaminaPercent() const { return BreathStamina / MaxBreathStamina; }

	// Check if currently holding breath
	UFUNCTION(BlueprintPure, Category = "Breath")
	bool IsHoldingBreath() const { return bIsHoldingBreath; }

	// Get camera sway vector (for camera shake)
	UFUNCTION(BlueprintPure, Category = "Breath")
	FVector GetBreathSway() const;

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	bool bIsHoldingBreath;
	float SwayTime;

	void UpdateBreathStamina(float DeltaTime);
	void UpdateSway(float DeltaTime);
};

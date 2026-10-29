// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "TDBFirstPersonCameraComponent.generated.h"

/**
 * First Person Camera Component with ADS (Aim Down Sights) support
 * Handles FOV transitions, head bobbing, and camera effects
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UTDBFirstPersonCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UTDBFirstPersonCameraComponent();

	// Current aim state (0 = hip fire, 1 = fully aimed)
	UPROPERTY(ReplicatedUsing = OnRep_AimState, EditAnywhere, BlueprintReadOnly, Category = "Aiming")
	float AimState;

	// Target aim state
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Aiming")
	float TargetAimState;

	// Current FOV
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Camera")
	float CurrentFOV;

	// Default FOV (hip fire)
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float DefaultFOV = 90.0f;

	// ADS FOV (when fully aimed)
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ADSFOV = 45.0f;

	// Interpolation speed for ADS transition
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	float AimInterpSpeed = 15.0f;

	// Head bob settings
	UPROPERTY(EditDefaultsOnly, Category = "Head Bob")
	float HeadBobWalkingIntensity = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Head Bob")
	float HeadBobRunningIntensity = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Head Bob")
	float HeadBobCrouchIntensity = 0.2f;

	// Camera lag
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	bool bEnableCameraLag = true;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraLagSpeed = 10.0f;

	// ADS control functions
	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void StartAiming();

	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void StopAiming();

	// Get current ADS alpha (0-1)
	UFUNCTION(BlueprintPure, Category = "Aiming")
	float GetAimAlpha() const { return AimState; }

	// Check if currently aiming
	UFUNCTION(BlueprintPure, Category = "Aiming")
	bool IsAiming() const { return AimState > 0.1f; }

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_AimState();

private:
	void UpdateFOV(float DeltaTime);
	void UpdateHeadBob(float DeltaTime);
	void UpdateCameraLag(float DeltaTime);

	// Head bob state
	float HeadBobTime = 0.0f;
	FVector HeadBobOffset = FVector::ZeroVector;
};

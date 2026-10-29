// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDBLeanComponent.generated.h"

/**
 * Lean Component - handles leaning left/right (Q/E keys)
 * Used for tactical peeking from cover
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UTDBLeanComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTDBLeanComponent();

	// Current lean state (-1 = left, 0 = center, 1 = right)
	UPROPERTY(ReplicatedUsing = OnRep_LeanState, BlueprintReadOnly, Category = "Lean")
	float LeanState;

	// Target lean state
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Lean")
	float TargetLeanState;

	// Maximum lean angle (degrees)
	UPROPERTY(EditAnywhere, Category = "Lean", meta = (ClampMin = "0.0", ClampMax = "45.0"))
	float MaxLeanAngle = 15.0f;

	// Maximum camera offset for lean
	UPROPERTY(EditAnywhere, Category = "Lean", meta = (ClampMin = "0.0"))
	float MaxLeanOffset = 30.0f;

	// Lean interpolation speed
	UPROPERTY(EditAnywhere, Category = "Lean", meta = (ClampMin = "0.0"))
	float LeanSpeed = 8.0f;

	// Current lean angle
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Lean")
	float CurrentLeanAngle;

	// Current lean offset
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Lean")
	float CurrentLeanOffset;

	// Lean functions
	UFUNCTION(BlueprintCallable, Category = "Lean")
	void StartLeanLeft();

	UFUNCTION(BlueprintCallable, Category = "Lean")
	void StartLeanRight();

	UFUNCTION(BlueprintCallable, Category = "Lean")
	void StopLean();

	// Get current lean angle
	UFUNCTION(BlueprintPure, Category = "Lean")
	float GetLeanAngle() const { return CurrentLeanAngle; }

	// Get current lean offset
	UFUNCTION(BlueprintPure, Category = "Lean")
	float GetLeanOffset() const { return CurrentLeanOffset; }

	// Check if currently leaning
	UFUNCTION(BlueprintPure, Category = "Lean")
	bool IsLeaning() const { return !FMath::IsNearlyZero(CurrentLeanAngle, 0.1f); }

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_LeanState();

private:
	void UpdateLean(float DeltaTime);
};

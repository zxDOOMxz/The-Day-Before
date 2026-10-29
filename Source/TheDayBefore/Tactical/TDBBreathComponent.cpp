// Copyright TheDayBefore Project. All Rights Reserved.

#include "Tactical/TDBBreathComponent.h"
#include "Net/UnrealNetwork.h"

UTDBBreathComponent::UTDBBreathComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	BreathStamina = MaxBreathStamina;
	BreathSwayIntensity = 0.0f;
	bIsHoldingBreath = false;
	SwayTime = 0.0f;
}

void UTDBBreathComponent::BeginPlay()
{
	Super::BeginPlay();
	BreathStamina = MaxBreathStamina;
}

void UTDBBreathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateBreathStamina(DeltaTime);
	UpdateSway(DeltaTime);
}

void UTDBBreathComponent::HoldBreath()
{
	// Can only hold breath if we have stamina
	if (BreathStamina > 0.0f)
	{
		bIsHoldingBreath = true;
	}
}

void UTDBBreathComponent::ReleaseBreath()
{
	bIsHoldingBreath = false;
}

void UTDBBreathComponent::UpdateBreathStamina(float DeltaTime)
{
	if (bIsHoldingBreath)
	{
		// Drain stamina
		BreathStamina = FMath::Clamp(BreathStamina - BreathDrainRate * DeltaTime, 0.0f, MaxBreathStamina);

		// Stop holding breath if stamina depleted
		if (BreathStamina <= 0.0f)
		{
			ReleaseBreath();
		}
	}
	else
	{
		// Recover stamina
		BreathStamina = FMath::Clamp(BreathStamina + BreathRecoveryRate * DeltaTime, 0.0f, MaxBreathStamina);
	}
}

void UTDBBreathComponent::UpdateSway(float DeltaTime)
{
	SwayTime += DeltaTime;

	// Calculate sway intensity based on stamina
	if (BreathStamina < SwayThreshold)
	{
		// Stamina below threshold, start sway
		float StaminaPercent = BreathStamina / SwayThreshold;
		BreathSwayIntensity = (1.0f - StaminaPercent) * MaxBreathSway;
	}
	else
	{
		// Stamina above threshold, no sway
		BreathSwayIntensity = FMath::FInterpTo(BreathSwayIntensity, 0.0f, DeltaTime, 5.0f);
	}

	// Additional sway when holding breath with low stamina
	if (bIsHoldingBreath && BreathStamina < 20.0f)
	{
		BreathSwayIntensity *= 1.5f;
	}
}

FVector UTDBBreathComponent::GetBreathSway() const
{
	if (FMath::IsNearlyZero(BreathSwayIntensity, 0.01f))
	{
		return FVector::ZeroVector;
	}

	// Generate sway offset using sine waves for smooth motion
	float SwayX = FMath::Sin(SwayTime * 1.5f) * BreathSwayIntensity;
	float SwayY = FMath::Cos(SwayTime * 1.2f) * BreathSwayIntensity * 0.7f;

	return FVector(0.0f, SwayY, SwayX);
}

void UTDBBreathComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTDBBreathComponent, BreathStamina);
}

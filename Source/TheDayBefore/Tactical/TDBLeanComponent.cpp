// Copyright TheDayBefore Project. All Rights Reserved.

#include "Tactical/TDBLeanComponent.h"
#include "Net/UnrealNetwork.h"
#include "Camera/TDBFirstPersonCameraComponent.h"

UTDBLeanComponent::UTDBLeanComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	LeanState = 0.0f;
	TargetLeanState = 0.0f;
	CurrentLeanAngle = 0.0f;
	CurrentLeanOffset = 0.0f;
}

void UTDBLeanComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTDBLeanComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateLean(DeltaTime);
}

void UTDBLeanComponent::StartLeanLeft()
{
	TargetLeanState = -1.0f;
}

void UTDBLeanComponent::StartLeanRight()
{
	TargetLeanState = 1.0f;
}

void UTDBLeanComponent::StopLean()
{
	TargetLeanState = 0.0f;
}

void UTDBLeanComponent::UpdateLean(float DeltaTime)
{
	// Interpolate lean state
	LeanState = FMath::FInterpTo(LeanState, TargetLeanState, DeltaTime, LeanSpeed);

	// Calculate target angle and offset
	float TargetAngle = LeanState * MaxLeanAngle;
	float TargetOffset = LeanState * MaxLeanOffset;

	// Interpolate actual values
	CurrentLeanAngle = FMath::FInterpTo(CurrentLeanAngle, TargetAngle, DeltaTime, LeanSpeed);
	CurrentLeanOffset = FMath::FInterpTo(CurrentLeanOffset, TargetOffset, DeltaTime, LeanSpeed);

	// Apply to camera if available
	APawn* Owner = Cast<APawn>(GetOwner());
	if (Owner)
	{
		UTDBFirstPersonCameraComponent* Camera = Owner->FindComponentByClass<UTDBFirstPersonCameraComponent>();
		if (Camera)
		{
			// Apply lean rotation and offset
			FRotator LeanRotation(0.0f, 0.0f, CurrentLeanAngle);
			Camera->AddRelativeRotation(LeanRotation);
			Camera->AddLocalOffset(FVector(0.0f, CurrentLeanOffset, 0.0f));
		}
	}
}

void UTDBLeanComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTDBLeanComponent, LeanState);
}

void UTDBLeanComponent::OnRep_LeanState()
{
	// Client-side interpolation
}

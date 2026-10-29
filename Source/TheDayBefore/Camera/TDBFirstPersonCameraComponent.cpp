// Copyright TheDayBefore Project. All Rights Reserved.

#include "Camera/TDBFirstPersonCameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UTDBFirstPersonCameraComponent::UTDBFirstPersonCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentFOV = DefaultFOV;
	AimState = 0.0f;
	TargetAimState = 0.0f;
}

void UTDBFirstPersonCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	SetFieldOfView(DefaultFOV);
}

void UTDBFirstPersonCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateFOV(DeltaTime);
	UpdateHeadBob(DeltaTime);
	UpdateCameraLag(DeltaTime);
}

void UTDBFirstPersonCameraComponent::StartAiming()
{
	TargetAimState = 1.0f;
}

void UTDBFirstPersonCameraComponent::StopAiming()
{
	TargetAimState = 0.0f;
}

void UTDBFirstPersonCameraComponent::UpdateFOV(float DeltaTime)
{
	// Interpolate aim state
	AimState = FMath::FInterpTo(AimState, TargetAimState, DeltaTime, AimInterpSpeed);

	// Calculate FOV based on aim state
	float TargetFOV = FMath::Lerp(DefaultFOV, ADSFOV, AimState);
	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, AimInterpSpeed);

	SetFieldOfView(CurrentFOV);
}

void UTDBFirstPersonCameraComponent::UpdateHeadBob(float DeltaTime)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!Owner)
		return;

	UCharacterMovementComponent* MovementComp = Owner->GetCharacterMovement();
	if (!MovementComp)
		return;

	// Only apply head bob when on ground
	if (!Owner->GetMesh() || !MovementComp->IsMovingOnGround())
	{
		HeadBobOffset = FMath::VInterpTo(HeadBobOffset, FVector::ZeroVector, DeltaTime, 10.0f);
		return;
	}

	// Determine intensity based on movement state
	float Intensity = 0.0f;
	float Speed = 0.0f;

	if (Owner->bIsCrouched)
	{
		Intensity = HeadBobCrouchIntensity;
		Speed = 8.0f;
	}
	else if (MovementComp->IsWalking())
	{
		Intensity = HeadBobRunningIntensity;
		Speed = 12.0f;
	}
	else
	{
		Intensity = HeadBobWalkingIntensity;
		Speed = 10.0f;
	}

	// Reduce intensity while aiming
	Intensity *= (1.0f - AimState);

	// Calculate head bob offset
	HeadBobTime += DeltaTime * Speed;
	float XOffset = FMath::Sin(HeadBobTime) * Intensity * 2.0f;
	float YOffset = FMath::Sin(HeadBobTime * 2.0f) * Intensity;

	HeadBobOffset = FVector(XOffset, YOffset, 0.0f);

	// Apply offset to camera
	AddRelativeLocation(HeadBobOffset * DeltaTime * 10.0f);
}

void UTDBFirstPersonCameraComponent::UpdateCameraLag(float DeltaTime)
{
	if (!bEnableCameraLag)
		return;

	// Camera lag is handled by spring arm or custom implementation
	// This is a simplified version
}

void UTDBFirstPersonCameraComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTDBFirstPersonCameraComponent, AimState);
}

void UTDBFirstPersonCameraComponent::OnRep_AimState()
{
	// Client-side interpolation will handle smooth transition
}

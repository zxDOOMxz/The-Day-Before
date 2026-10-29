// Copyright TheDayBefore Project. All Rights Reserved.

#include "Characters/TDBCharacter.h"
#include "Camera/FirstPersonCameraComponent.h"
#include "Tactical/LeanComponent.h"
#include "Tactical/BreathComponent.h"
#include "Weapons/TDBWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

ATDBCharacter::ATDBCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create First Person Camera
	FirstPersonCameraComponent = CreateDefaultSubobject<UFirstPersonCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 64.f)); // Eye height
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create Arms Mesh
	ArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMesh"));
	ArmsMesh->SetOnlyOwnerSee(true);
	ArmsMesh->SetupAttachment(FirstPersonCameraComponent);
	ArmsMesh->bCastDynamicShadow = false;
	ArmsMesh->CastShadow = false;
	ArmsMesh->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Create tactical components
	LeanComponent = CreateDefaultSubobject<ULeanComponent>(TEXT("LeanComponent"));
	BreathComponent = CreateDefaultSubobject<UBreathComponent>(TEXT("BreathComponent"));

	// Create weapon component
	WeaponComponent = CreateDefaultSubobject<UTDBWeaponComponent>(TEXT("WeaponComponent"));

	// Configure input
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ATDBCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetupArmsMesh();
}

void ATDBCharacter::SetupArmsMesh()
{
	if (ArmsMesh)
	{
		// Arms mesh should not cast shadows for first person view
		ArmsMesh->SetVisibility(true);
	}
}

void ATDBCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ATDBCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ATDBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATDBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Input actions will be bound here
		// TODO: Add Input Mapping Context
	}
}

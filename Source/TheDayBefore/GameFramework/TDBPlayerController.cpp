// Copyright TheDayBefore Project. All Rights Reserved.

#include "GameFramework/TDBPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Characters/TDBCharacter.h"
#include "Camera/TDBFirstPersonCameraComponent.h"
#include "Tactical/TDBLeanComponent.h"
#include "Tactical/TDBBreathComponent.h"
#include "Weapons/TDBWeaponComponent.h"
#include "Inventory/TDBInventoryComponent.h"
#include "Inventory/TDBItemPickup.h"
#include "Kismet/GameplayStatics.h"

ATDBPlayerController::ATDBPlayerController()
{
	// Enable mouse input by default
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
}

void ATDBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetupInputMapping();
}

void ATDBPlayerController::SetupInputMapping()
{
	if (IsLocalController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void ATDBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Movement
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATDBPlayerController::HandleMove);
		}

		// Look
		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATDBPlayerController::HandleLook);
		}

		// Jump
		if (JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ATDBPlayerController::HandleJump);
		}

		// Crouch
		if (CrouchAction)
		{
			EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &ATDBPlayerController::HandleCrouch);
		}

		// Sprint
		if (SprintAction)
		{
			EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &ATDBPlayerController::HandleSprint);
			EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATDBPlayerController::HandleSprint);
		}

		// Aim (ADS)
		if (AimAction)
		{
			EnhancedInput->BindAction(AimAction, ETriggerEvent::Started, this, &ATDBPlayerController::HandleAim);
			EnhancedInput->BindAction(AimAction, ETriggerEvent::Completed, this, &ATDBPlayerController::HandleAimReleased);
		}

		// Lean
		if (LeanLeftAction)
		{
			EnhancedInput->BindAction(LeanLeftAction, ETriggerEvent::Started, this, &ATDBPlayerController::HandleLeanLeft);
			EnhancedInput->BindAction(LeanLeftAction, ETriggerEvent::Completed, this, &ATDBPlayerController::HandleLeanLeftReleased);
		}

		if (LeanRightAction)
		{
			EnhancedInput->BindAction(LeanRightAction, ETriggerEvent::Started, this, &ATDBPlayerController::HandleLeanRight);
			EnhancedInput->BindAction(LeanRightAction, ETriggerEvent::Completed, this, &ATDBPlayerController::HandleLeanRightReleased);
		}

		// Hold Breath
		if (HoldBreathAction)
		{
			EnhancedInput->BindAction(HoldBreathAction, ETriggerEvent::Started, this, &ATDBPlayerController::HandleHoldBreath);
			EnhancedInput->BindAction(HoldBreathAction, ETriggerEvent::Completed, this, &ATDBPlayerController::HandleHoldBreathReleased);
		}

		// Interact
		if (InteractAction)
		{
			EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &ATDBPlayerController::HandleInteract);
		}

		// Inventory
		if (InventoryAction)
		{
			EnhancedInput->BindAction(InventoryAction, ETriggerEvent::Started, this, &ATDBPlayerController::HandleInventory);
		}

		// Fire
		if (FireAction)
		{
			EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &ATDBPlayerController::HandleFire);
			EnhancedInput->BindAction(FireAction, ETriggerEvent::Completed, this, &ATDBPlayerController::HandleFireReleased);
		}

		// Reload
		if (ReloadAction)
		{
			EnhancedInput->BindAction(ReloadAction, ETriggerEvent::Started, this, &ATDBPlayerController::HandleReload);
		}
	}
}

void ATDBPlayerController::HandleMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (GetPawn() != nullptr)
	{
		const FRotator Rotation = GetPawn()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		GetPawn()->AddMovementInput(ForwardDirection, MovementVector.Y);
		GetPawn()->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATDBPlayerController::HandleLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetPawn() != nullptr)
	{
		AddYawInput(LookAxisVector.X);
		AddPitchInput(LookAxisVector.Y);
	}
}

void ATDBPlayerController::HandleJump(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->Jump();
	}
}

void ATDBPlayerController::HandleCrouch(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		if (PlayerCharacter->bIsCrouched)
		{
			PlayerCharacter->UnCrouch();
		}
		else
		{
			PlayerCharacter->Crouch();
		}
	}
}

void ATDBPlayerController::HandleSprint(const FInputActionValue& Value)
{
	// TODO: Implement sprint logic
}

void ATDBPlayerController::HandleAim(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (PlayerCharacter && PlayerCharacter->FirstPersonCameraComponent)
	{
		PlayerCharacter->FirstPersonCameraComponent->StartAiming();
	}
}

void ATDBPlayerController::HandleAimReleased(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (PlayerCharacter && PlayerCharacter->FirstPersonCameraComponent)
	{
		PlayerCharacter->FirstPersonCameraComponent->StopAiming();
	}
}

void ATDBPlayerController::HandleLeanLeft(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (PlayerCharacter && PlayerCharacter->LeanComponent)
	{
		PlayerCharacter->LeanComponent->StartLeanLeft();
	}
}

void ATDBPlayerController::HandleLeanLeftReleased(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (PlayerCharacter && PlayerCharacter->LeanComponent)
	{
		PlayerCharacter->LeanComponent->StopLean();
	}
}

void ATDBPlayerController::HandleLeanRight(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (PlayerCharacter && PlayerCharacter->LeanComponent)
	{
		PlayerCharacter->LeanComponent->StartLeanRight();
	}
}

void ATDBPlayerController::HandleLeanRightReleased(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (PlayerCharacter && PlayerCharacter->LeanComponent)
	{
		PlayerCharacter->LeanComponent->StopLean();
	}
}

void ATDBPlayerController::HandleHoldBreath(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (PlayerCharacter && PlayerCharacter->BreathComponent)
	{
		PlayerCharacter->BreathComponent->HoldBreath();
	}
}

void ATDBPlayerController::HandleHoldBreathReleased(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (PlayerCharacter && PlayerCharacter->BreathComponent)
	{
		PlayerCharacter->BreathComponent->ReleaseBreath();
	}
}

void ATDBPlayerController::HandleInventory(const FInputActionValue& Value)
{
	// TODO: Toggle inventory UI
}

void ATDBPlayerController::HandleFire(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (PlayerCharacter && PlayerCharacter->WeaponComponent)
	{
		PlayerCharacter->WeaponComponent->Fire();
	}
}

void ATDBPlayerController::HandleFireReleased(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (PlayerCharacter && PlayerCharacter->WeaponComponent)
	{
		PlayerCharacter->WeaponComponent->StopFire();
	}
}

void ATDBPlayerController::HandleReload(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (PlayerCharacter && PlayerCharacter->WeaponComponent)
	{
		PlayerCharacter->WeaponComponent->Reload();
	}
}

void ATDBPlayerController::HandleInteract(const FInputActionValue& Value)
{
	ATDBCharacter* PlayerCharacter = Cast<ATDBCharacter>(GetPawn());
	if (!PlayerCharacter || !PlayerCharacter->InventoryComponent)
	{
		return;
	}

	// Find nearby item pickup
	// TODO: Implement proper interaction system with trace
	// For now, this is a placeholder - will need line trace to find nearest pickup
	
	UE_LOG(LogTemp, Log, TEXT("Interact pressed - pickup system needs implementation"));
	
	// Example: Find all pickups in radius and pick up nearest one
	// This should be replaced with proper interaction trace
	TArray<AActor*> NearbyPickups;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATDBItemPickup::StaticClass(), NearbyPickups);
	
	float MinDistance = 200.0f; // 2 meters
	ATDBItemPickup* NearestPickup = nullptr;
	
	for (AActor* Actor : NearbyPickups)
	{
		ATDBItemPickup* Pickup = Cast<ATDBItemPickup>(Actor);
		if (Pickup)
		{
			float Distance = FVector::Dist(PlayerCharacter->GetActorLocation(), Pickup->GetActorLocation());
			if (Distance < MinDistance && Pickup->CanBePickedUp(PlayerCharacter->InventoryComponent))
			{
				MinDistance = Distance;
				NearestPickup = Pickup;
			}
		}
	}
	
	if (NearestPickup)
	{
		NearestPickup->PickUp(PlayerCharacter->InventoryComponent);
	}
}

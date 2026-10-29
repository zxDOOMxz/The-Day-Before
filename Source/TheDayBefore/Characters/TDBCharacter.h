// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TDBCharacter.generated.h"

class UFirstPersonCameraComponent;
class ULeanComponent;
class UBreathComponent;
class USpringArmComponent;
class UTDBWeaponComponent;
class UTDBInventoryComponent;

/**
 * Base character class for The Day Before
 * First-person perspective with tactical mechanics
 */
UCLASS(Blueprintable)
class ATDBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATDBCharacter();

	// FPS Camera Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UFirstPersonCameraComponent* FirstPersonCameraComponent;

	// Arms mesh (visible hands with weapon)
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ArmsMesh;

	// Lean component (Q/E lean from cover)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tactical", meta = (AllowPrivateAccess = "true"))
	ULeanComponent* LeanComponent;

	// Breath component (hold breath while aiming)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tactical", meta = (AllowPrivateAccess = "true"))
	UBreathComponent* BreathComponent;

	// Weapon component (manages weapon holding)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UTDBWeaponComponent* WeaponComponent;

	// Inventory component (manages items and equipment)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UTDBInventoryComponent* InventoryComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Getters
	UFUNCTION(BlueprintPure, Category = "Camera")
	UFirstPersonCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// APawn interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	// End of APawn interface

private:
	void SetupArmsMesh();
};

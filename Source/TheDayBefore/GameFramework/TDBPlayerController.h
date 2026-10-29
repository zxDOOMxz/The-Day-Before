// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDBPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * Base PlayerController for The Day Before
 * Handles input, UI, and player-specific game logic
 */
UCLASS()
class ATDBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATDBPlayerController();

	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	// Input Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* AimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LeanLeftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LeanRightAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* HoldBreathAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* InventoryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* ReloadAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Input handlers
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleJump(const FInputActionValue& Value);
	void HandleCrouch(const FInputActionValue& Value);
	void HandleSprint(const FInputActionValue& Value);
	void HandleAim(const FInputActionValue& Value);
	void HandleAimReleased(const FInputActionValue& Value);
	void HandleLeanLeft(const FInputActionValue& Value);
	void HandleLeanLeftReleased(const FInputActionValue& Value);
	void HandleLeanRight(const FInputActionValue& Value);
	void HandleLeanRightReleased(const FInputActionValue& Value);
	void HandleHoldBreath(const FInputActionValue& Value);
	void HandleHoldBreathReleased(const FInputActionValue& Value);
	void HandleInteract(const FInputActionValue& Value);
	void HandleInventory(const FInputActionValue& Value);
	void HandleFire(const FInputActionValue& Value);
	void HandleFireReleased(const FInputActionValue& Value);
	void HandleReload(const FInputActionValue& Value);
	void HandleInteract(const FInputActionValue& Value);

private:
	void SetupInputMapping();
};

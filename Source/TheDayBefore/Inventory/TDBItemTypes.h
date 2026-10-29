// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TDBItemTypes.generated.h"

/**
 * Item category enum
 */
UENUM(BlueprintType)
enum class EItemCategory : uint8
{
    Weapon      UMETA(DisplayName = "Weapon"),
    Armor       UMETA(DisplayName = "Armor"),
    Ammo        UMETA(DisplayName = "Ammo"),
    Medical     UMETA(DisplayName = "Medical"),
    Food        UMETA(DisplayName = "Food"),
    Resource    UMETA(DisplayName = "Resource"),
    Key         UMETA(DisplayName = "Key"),
    Quest       UMETA(DisplayName = "Quest Item"),
    Misc        UMETA(DisplayName = "Miscellaneous")
};

/**
 * Item rarity enum
 */
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    Common      UMETA(DisplayName = "Common", Color = "Gray"),
    Uncommon    UMETA(DisplayName = "Uncommon", Color = "Green"),
    Rare        UMETA(DisplayName = "Rare", Color = "Blue"),
    Epic        UMETA(DisplayName = "Epic", Color = "Purple"),
    Legendary   UMETA(DisplayName = "Legendary", Color = "Orange")
};

/**
 * Item definition - static data from DataTable
 */
USTRUCT(BlueprintType)
struct FItemDefinition : public FTableRowBase
{
    GENERATED_BODY()

public:
    // Unique item ID
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FName ItemId;

    // Display name
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FText DisplayName;

    // Description
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FText Description;

    // Icon texture
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UTexture2D> Icon;

    // 3D mesh for world pickup
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UStaticMesh> WorldMesh;

    // Inventory size (X, Y) in grid slots
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FIntPoint InventorySize = FIntPoint(1, 1);

    // Weight in kg
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    float Weight = 1.0f;

    // Item category
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    EItemCategory Category = EItemCategory::Misc;

    // Item rarity
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    EItemRarity Rarity = EItemRarity::Common;

    // Maximum stack size (0 = not stackable)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    int32 MaxStackSize = 1;

    // Base value (currency)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    int32 BaseValue = 100;

    // Is this item stackable?
    bool IsStackable() const { return MaxStackSize > 1; }
};

/**
 * Item instance - runtime data for a specific item
 */
USTRUCT(BlueprintType)
struct FItemInstance
{
    GENERATED_BODY()

public:
    // Unique instance ID
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item", Meta = (IgnoreForMemberInitializationTest))
    FGuid InstanceId;

    // Reference to item definition
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    FName ItemId;

    // Stack count
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    int32 Quantity = 1;

    // Condition (0-100%, affects weapon accuracy, armor protection)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    float Condition = 100.0f;

    // Custom properties (ammo count, durability, etc.)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    TMap<FName, float> CustomProperties;

    FItemInstance()
    {
        InstanceId = FGuid::NewGuid();
    }

    FItemInstance(FName InItemId, int32 InQuantity = 1)
        : ItemId(InItemId)
        , Quantity(InQuantity)
    {
        InstanceId = FGuid::NewGuid();
    }

    // Check if this instance is valid
    bool IsValid() const { return !ItemId.IsNone() && Quantity > 0; }

    // Add to stack (returns leftover if stack is full)
    int32 AddToStack(int32 Amount, int32 MaxStackSize)
    {
        if (MaxStackSize <= 0) return Amount; // Not stackable
        
        int32 SpaceLeft = MaxStackSize - Quantity;
        int32 AmountToAdd = FMath::Min(Amount, SpaceLeft);
        Quantity += AmountToAdd;
        return Amount - AmountToAdd; // Return leftover
    }

    // Remove from stack (returns actual removed amount)
    int32 RemoveFromStack(int32 Amount)
    {
        int32 AmountToRemove = FMath::Min(Amount, Quantity);
        Quantity -= AmountToRemove;
        return AmountToRemove;
    }
};

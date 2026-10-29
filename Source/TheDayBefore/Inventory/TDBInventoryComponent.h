// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/TDBItemTypes.h"
#include "TDBInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, const TArray<FItemInstance>&, Items);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAdded, FName, ItemId, int32, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemRemoved, FName, ItemId, int32, Quantity);

/**
 * Inventory component - manages item storage and organization
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UTDBInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTDBInventoryComponent();

    // Inventory grid size (X, Y slots)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    FIntPoint GridSize = FIntPoint(10, 5); // 50 slots total

    // Maximum weight capacity (kg)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    float MaxWeight = 100.0f;

    // Current items
    UPROPERTY(ReplicatedUsing = OnRep_Items, BlueprintReadOnly, Category = "Inventory")
    TArray<FItemInstance> Items;

    // Item definitions table
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    UDataTable* ItemDefinitionsTable;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryChanged OnInventoryChanged;

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnItemAdded OnItemAdded;

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnItemRemoved OnItemRemoved;

    // Add item to inventory
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(FName ItemId, int32 Quantity = 1);

    // Remove item from inventory
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(FName ItemId, int32 Quantity = 1);

    // Check if inventory has item
    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool HasItem(FName ItemId, int32 Quantity = 1) const;

    // Get item count
    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetItemCount(FName ItemId) const;

    // Get item definition by ID (returns pointer to struct - not exposed to Blueprints)
    const FItemDefinition* GetItemDefinition(FName ItemId) const;

    // Get current weight
    UFUNCTION(BlueprintPure, Category = "Inventory")
    float GetCurrentWeight() const;

    // Check if inventory can accept item
    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool CanAddItem(FName ItemId, int32 Quantity = 1) const;

    // Clear inventory
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void ClearInventory();

    // Get all items
    UFUNCTION(BlueprintPure, Category = "Inventory")
    const TArray<FItemInstance>& GetAllItems() const { return Items; }

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void OnRep_Items();

private:
    float CalculateCurrentWeight() const;
    bool FindItemStack(FName ItemId, FItemInstance*& OutItem);
    const FItemInstance* FindItemStack(FName ItemId) const;
};

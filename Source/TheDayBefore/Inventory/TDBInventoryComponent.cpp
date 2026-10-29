// Copyright TheDayBefore Project. All Rights Reserved.

#include "Inventory/TDBInventoryComponent.h"
#include "Net/UnrealNetwork.h"

UTDBInventoryComponent::UTDBInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UTDBInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UTDBInventoryComponent::AddItem(FName ItemId, int32 Quantity)
{
    if (ItemId.IsNone() || Quantity <= 0)
    {
        return false;
    }

    // Check if we can add this item
    if (!CanAddItem(ItemId, Quantity))
    {
        return false;
    }

    const FItemDefinition* ItemDef = GetItemDefinition(ItemId);
    if (!ItemDef)
    {
        UE_LOG(LogTemp, Warning, TEXT("Item definition not found: %s"), *ItemId.ToString());
        return false;
    }

    // Try to stack with existing items
    if (ItemDef->IsStackable())
    {
        FItemInstance* ExistingStack = nullptr;
        if (FindItemStack(ItemId, ExistingStack))
        {
            int32 Leftover = ExistingStack->AddToStack(Quantity, ItemDef->MaxStackSize);
            
            if (Leftover == 0)
            {
                // Successfully added all to existing stack
                OnItemAdded.Broadcast(ItemId, Quantity);
                OnInventoryChanged.Broadcast(Items);
                return true;
            }
            else
            {
                // Stack is full, create new instance with leftover
                Quantity = Leftover;
            }
        }
    }

    // Create new item instance
    FItemInstance NewItem(ItemId, Quantity);
    Items.Add(NewItem);

    OnItemAdded.Broadcast(ItemId, Quantity);
    OnInventoryChanged.Broadcast(Items);

    UE_LOG(LogTemp, Log, TEXT("Added item: %s x%d"), *ItemId.ToString(), Quantity);
    return true;
}

bool UTDBInventoryComponent::RemoveItem(FName ItemId, int32 Quantity)
{
    if (ItemId.IsNone() || Quantity <= 0)
    {
        return false;
    }

    if (!HasItem(ItemId, Quantity))
    {
        return false;
    }

    int32 RemainingToRemove = Quantity;

    // Remove from stacks
    for (int32 i = Items.Num() - 1; i >= 0; --i)
    {
        if (Items[i].ItemId == ItemId && RemainingToRemove > 0)
        {
            int32 Removed = Items[i].RemoveFromStack(RemainingToRemove);
            RemainingToRemove -= Removed;

            // Remove empty stacks
            if (Items[i].Quantity <= 0)
            {
                Items.RemoveAt(i);
            }

            if (RemainingToRemove <= 0)
            {
                break;
            }
        }
    }

    OnItemRemoved.Broadcast(ItemId, Quantity);
    OnInventoryChanged.Broadcast(Items);

    UE_LOG(LogTemp, Log, TEXT("Removed item: %s x%d"), *ItemId.ToString(), Quantity);
    return true;
}

bool UTDBInventoryComponent::HasItem(FName ItemId, int32 Quantity) const
{
    return GetItemCount(ItemId) >= Quantity;
}

int32 UTDBInventoryComponent::GetItemCount(FName ItemId) const
{
    int32 TotalCount = 0;
    for (const FItemInstance& Item : Items)
    {
        if (Item.ItemId == ItemId)
        {
            TotalCount += Item.Quantity;
        }
    }
    return TotalCount;
}

const FItemDefinition* UTDBInventoryComponent::GetItemDefinition(FName ItemId) const
{
    if (!ItemDefinitionsTable)
    {
        return nullptr;
    }

    return ItemDefinitionsTable->FindRow<FItemDefinition>(ItemId, TEXT("GetItemDefinition"));
}

float UTDBInventoryComponent::GetCurrentWeight() const
{
    return CalculateCurrentWeight();
}

bool UTDBInventoryComponent::CanAddItem(FName ItemId, int32 Quantity) const
{
    const FItemDefinition* ItemDef = GetItemDefinition(ItemId);
    if (!ItemDef)
    {
        return false;
    }

    // Check weight
    float ItemWeight = ItemDef->Weight * Quantity;
    float CurrentWeight = CalculateCurrentWeight();
    
    if (CurrentWeight + ItemWeight > MaxWeight)
    {
        return false;
    }

    // Check grid space (simplified - real implementation would check actual slots)
    // For now, just check if we have any space
    int32 TotalSlots = GridSize.X * GridSize.Y;
    if (Items.Num() >= TotalSlots && !ItemDef->IsStackable())
    {
        return false;
    }

    return true;
}

void UTDBInventoryComponent::ClearInventory()
{
    Items.Empty();
    OnInventoryChanged.Broadcast(Items);
    UE_LOG(LogTemp, Log, TEXT("Inventory cleared"));
}

float UTDBInventoryComponent::CalculateCurrentWeight() const
{
    float TotalWeight = 0.0f;
    for (const FItemInstance& Item : Items)
    {
        const FItemDefinition* ItemDef = GetItemDefinition(Item.ItemId);
        if (ItemDef)
        {
            TotalWeight += ItemDef->Weight * Item.Quantity;
        }
    }
    return TotalWeight;
}

bool UTDBInventoryComponent::FindItemStack(FName ItemId, FItemInstance*& OutItem)
{
    const FItemDefinition* ItemDef = GetItemDefinition(ItemId);
    if (!ItemDef || !ItemDef->IsStackable())
    {
        return false;
    }

    for (FItemInstance& Item : Items)
    {
        if (Item.ItemId == ItemId && Item.Quantity < ItemDef->MaxStackSize)
        {
            OutItem = &Item;
            return true;
        }
    }
    return false;
}

const FItemInstance* UTDBInventoryComponent::FindItemStack(FName ItemId) const
{
    const FItemDefinition* ItemDef = GetItemDefinition(ItemId);
    if (!ItemDef || !ItemDef->IsStackable())
    {
        return nullptr;
    }

    for (const FItemInstance& Item : Items)
    {
        if (Item.ItemId == ItemId && Item.Quantity < ItemDef->MaxStackSize)
        {
            return &Item;
        }
    }
    return nullptr;
}

void UTDBInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UTDBInventoryComponent, Items);
}

void UTDBInventoryComponent::OnRep_Items()
{
    OnInventoryChanged.Broadcast(Items);
}

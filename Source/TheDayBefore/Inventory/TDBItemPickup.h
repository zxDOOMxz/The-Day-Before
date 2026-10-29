// Copyright TheDayBefore Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory/TDBItemTypes.h"
#include "TDBItemPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/**
 * Item pickup actor - represents items in the world that can be picked up
 */
UCLASS()
class ATDBItemPickup : public AActor
{
    GENERATED_BODY()

public:
    ATDBItemPickup();

    // Collision sphere for interaction
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
    USphereComponent* CollisionSphere;

    // Mesh component for visual representation
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
    UStaticMeshComponent* MeshComponent;

    // Item ID to spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    FName ItemId;

    // Quantity of items
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    int32 Quantity = 1;

    // Item definitions table (for getting item data)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
    UDataTable* ItemDefinitionsTable;

    // Initialize pickup with item data
    UFUNCTION(BlueprintCallable, Category = "Pickup")
    void InitializePickup(FName InItemId, int32 InQuantity = 1);

    // Pick up this item
    UFUNCTION(BlueprintCallable, Category = "Pickup")
    bool PickUp(class UTDBInventoryComponent* Inventory);

    // Check if can be picked up
    UFUNCTION(BlueprintPure, Category = "Pickup")
    bool CanBePickedUp(class UTDBInventoryComponent* Inventory) const;

    // Get item display name
    UFUNCTION(BlueprintPure, Category = "Pickup")
    FText GetDisplayName() const;

protected:
    virtual void BeginPlay() override;

    // Overlap event for interaction prompt
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    bool bIsPickedUp = false;
};

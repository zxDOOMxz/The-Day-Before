// Copyright TheDayBefore Project. All Rights Reserved.

#include "Inventory/TDBItemPickup.h"
#include "Inventory/TDBInventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"
#include "Characters/TDBCharacter.h"

ATDBItemPickup::ATDBItemPickup()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create collision sphere
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->SetSphereRadius(50.0f);
    CollisionSphere->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = CollisionSphere;

    // Create mesh component
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

    // Bind overlap events
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATDBItemPickup::OnOverlapBegin);
    CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ATDBItemPickup::OnOverlapEnd);
}

void ATDBItemPickup::BeginPlay()
{
    Super::BeginPlay();

    // Set mesh if item definition is available
    if (ItemDefinitionsTable && !ItemId.IsNone())
    {
        const FItemDefinition* ItemDef = ItemDefinitionsTable->FindRow<FItemDefinition>(
            ItemId, TEXT("BeginPlay"));
        
        if (ItemDef && !ItemDef->WorldMesh.IsNull())
        {
            MeshComponent->SetStaticMesh(ItemDef->WorldMesh.Get());
        }
    }
}

void ATDBItemPickup::InitializePickup(FName InItemId, int32 InQuantity)
{
    ItemId = InItemId;
    Quantity = InQuantity;

    // Update mesh if table is set
    if (ItemDefinitionsTable && !ItemId.IsNone())
    {
        const FItemDefinition* ItemDef = ItemDefinitionsTable->FindRow<FItemDefinition>(
            ItemId, TEXT("InitializePickup"));
        
        if (ItemDef && !ItemDef->WorldMesh.IsNull())
        {
            MeshComponent->SetStaticMesh(ItemDef->WorldMesh.Get());
        }
    }
}

bool ATDBItemPickup::PickUp(UTDBInventoryComponent* Inventory)
{
    if (!Inventory || bIsPickedUp || ItemId.IsNone())
    {
        return false;
    }

    if (!CanBePickedUp(Inventory))
    {
        return false;
    }

    // Add to inventory
    bool bSuccess = Inventory->AddItem(ItemId, Quantity);
    
    if (bSuccess)
    {
        bIsPickedUp = true;
        
        // Play pickup sound/effect here if needed
        
        // Destroy actor
        Destroy();
        
        UE_LOG(LogTemp, Log, TEXT("Picked up item: %s x%d"), *ItemId.ToString(), Quantity);
        return true;
    }

    return false;
}

bool ATDBItemPickup::CanBePickedUp(UTDBInventoryComponent* Inventory) const
{
    if (!Inventory || bIsPickedUp || ItemId.IsNone())
    {
        return false;
    }

    return Inventory->CanAddItem(ItemId, Quantity);
}

FText ATDBItemPickup::GetDisplayName() const
{
    if (ItemDefinitionsTable && !ItemId.IsNone())
    {
        const FItemDefinition* ItemDef = ItemDefinitionsTable->FindRow<FItemDefinition>(
            ItemId, TEXT("GetDisplayName"));
        
        if (ItemDef)
        {
            return FText::Format(FText::FromString("{0} x{1}"), ItemDef->DisplayName, Quantity);
        }
    }

    return FText::FromString("Unknown Item");
}

void ATDBItemPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                     bool bFromSweep, const FHitResult& SweepResult)
{
    ATDBCharacter* Character = Cast<ATDBCharacter>(OtherActor);
    if (Character)
    {
        // Show interaction prompt
        // TODO: Implement UI prompt system
        UE_LOG(LogTemp, Log, TEXT("Player near pickup: %s"), *ItemId.ToString());
    }
}

void ATDBItemPickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Hide interaction prompt
    // TODO: Implement UI prompt system
}

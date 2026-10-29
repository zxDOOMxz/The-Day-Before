// Copyright TheDayBefore Project. All Rights Reserved.

#include "Weapons/TDBWeaponComponent.h"
#include "Weapons/TDBWeaponBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

UTDBWeaponComponent::UTDBWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UTDBWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// Equip default weapon if available
	if (AvailableWeapons.Num() > 0 && !CurrentWeapon)
	{
		EquipWeapon(AvailableWeapons[0]);
	}
}

void UTDBWeaponComponent::EquipWeapon(TSubclassOf<ATDBWeaponBase> WeaponClass)
{
	if (!WeaponClass)
	{
		return;
	}

	// Unequip current weapon if any
	if (CurrentWeapon)
	{
		UnequipWeapon();
	}

	// Spawn new weapon
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetOwner()->GetInstigator();

	CurrentWeapon = World->SpawnActor<ATDBWeaponBase>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (CurrentWeapon)
	{
		AttachWeaponToCharacter(CurrentWeapon);
	}
}

void UTDBWeaponComponent::UnequipWeapon()
{
	if (!CurrentWeapon)
	{
		return;
	}

	DetachWeaponFromCharacter(CurrentWeapon);
	CurrentWeapon->Destroy();
	CurrentWeapon = nullptr;
}

void UTDBWeaponComponent::AttachWeaponToCharacter(ATDBWeaponBase* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh)
	{
		return;
	}

	// Attach weapon to socket
	Weapon->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
}

void UTDBWeaponComponent::DetachWeaponFromCharacter(ATDBWeaponBase* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void UTDBWeaponComponent::Fire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void UTDBWeaponComponent::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void UTDBWeaponComponent::Reload()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartReload();
	}
}

void UTDBWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTDBWeaponComponent, CurrentWeapon);
}

void UTDBWeaponComponent::OnRep_CurrentWeapon()
{
	// Client-side weapon update
	if (CurrentWeapon)
	{
		AttachWeaponToCharacter(CurrentWeapon);
	}
}

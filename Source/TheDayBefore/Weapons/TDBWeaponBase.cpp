// Copyright TheDayBefore Project. All Rights Reserved.

#include "Weapons/TDBWeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/GameInstance.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Combat/TDBDamageSystem.h"

ATDBWeaponBase::ATDBWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create weapon mesh
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	// Create muzzle flash
	MuzzleFlash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MuzzleFlash"));
	MuzzleFlash->SetupAttachment(WeaponMesh, FName("Muzzle"));
	MuzzleFlash->bAutoActivate = false;

	// Create audio component
	WeaponAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("WeaponAudio"));
	WeaponAudio->SetupAttachment(RootComponent);

	// Network settings
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ATDBWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATDBWeaponBase::Fire()
{
	if (!CanFire() || bIsReloading)
	{
		return;
	}

	bIsFiring = true;

	if (bIsAutomatic)
	{
		// Automatic fire
		FireWeapon();
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATDBWeaponBase::FireWeapon, GetTimeBetweenShots(), true);
	}
	else
	{
		// Semi-automatic or bolt-action
		FireWeapon();
	}
}

void ATDBWeaponBase::StopFire()
{
	bIsFiring = false;
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void ATDBWeaponBase::FireWeapon()
{
	if (!CanFire())
	{
		StopFire();
		return;
	}

	// Decrease ammo
	CurrentAmmo--;

	// Play fire effects
	PlayFireEffects();

	// Apply recoil
	ApplyRecoil();

	// Server-side hit detection
	if (GetLocalRole() == ROLE_Authority)
	{
		// Perform raycast for hit detection
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		APlayerController* PC = Cast<APlayerController>(OwnerCharacter ? OwnerCharacter->GetController() : nullptr);
		
		if (PC)
		{
			FVector CameraLoc;
			FRotator CameraRot;
			PC->GetPlayerViewPoint(CameraLoc, CameraRot);

			// Apply accuracy spread
			float Spread = (1.0f - Accuracy) * 100.0f;
			CameraRot.Pitch += FMath::FRandRange(-Spread, Spread);
			CameraRot.Yaw += FMath::FRandRange(-Spread, Spread);

			FVector Direction = CameraRot.Vector();
			FVector TraceEnd = CameraLoc + (Direction * MaxRange);

			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			if (OwnerCharacter)
			{
				QueryParams.AddIgnoredActor(OwnerCharacter);
			}

			if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLoc, TraceEnd, ECC_Visibility, QueryParams))
			{
				// Apply damage
				UTDBDamageSystem* DamageSystem = GetGameInstance()->GetSubsystem<UTDBDamageSystem>();
				if (DamageSystem && HitResult.GetActor())
				{
					DamageSystem->ApplyDamage(
						HitResult.GetActor(),
						BaseDamage,
						PC,
						HitResult.ImpactPoint,
						HitResult
					);
				}
			}
		}
	}

	// Auto-reload if empty
	if (CurrentAmmo <= 0)
	{
		StopFire();
		StartReload();
	}
}

void ATDBWeaponBase::ApplyRecoil()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	APlayerController* PC = Cast<APlayerController>(OwnerCharacter ? OwnerCharacter->GetController() : nullptr);

	if (PC)
	{
		// Apply vertical recoil
		FRotator CurrentRot = PC->GetControlRotation();
		CurrentRot.Pitch += RecoilVertical;
		CurrentRot.Yaw += FMath::FRandRange(-RecoilHorizontal, RecoilHorizontal);
		PC->SetControlRotation(CurrentRot);
	}
}

void ATDBWeaponBase::PlayFireEffects()
{
	// Play muzzle flash
	if (MuzzleFlash)
	{
		MuzzleFlash->Activate(true);
	}

	// Play fire sound
	if (WeaponAudio)
	{
		WeaponAudio->Play();
	}

	// TODO: Play fire animation
}

bool ATDBWeaponBase::CanFire() const
{
	return CurrentAmmo > 0 && !bIsReloading && !bIsFiring;
}

bool ATDBWeaponBase::CanReload() const
{
	return CurrentAmmo < MagazineSize && ReserveAmmo > 0 && !bIsReloading;
}

void ATDBWeaponBase::StartReload()
{
	if (!CanReload())
	{
		return;
	}

	bIsReloading = true;

	// Start reload timer
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &ATDBWeaponBase::FinishReload, ReloadTime, false);

	// TODO: Play reload animation and sound
}

void ATDBWeaponBase::FinishReload()
{
	// Calculate how much ammo to reload
	int32 AmmoNeeded = MagazineSize - CurrentAmmo;
	int32 AmmoToReload = FMath::Min(AmmoNeeded, ReserveAmmo);

	CurrentAmmo += AmmoToReload;
	ReserveAmmo -= AmmoToReload;

	bIsReloading = false;

	// TODO: Play reload finish sound
}

void ATDBWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATDBWeaponBase, bIsFiring);
	DOREPLIFETIME(ATDBWeaponBase, bIsReloading);
	DOREPLIFETIME(ATDBWeaponBase, CurrentAmmo);
	DOREPLIFETIME(ATDBWeaponBase, ReserveAmmo);
}

void ATDBWeaponBase::OnRep_IsFiring()
{
	if (bIsFiring)
	{
		PlayFireEffects();
	}
}

void ATDBWeaponBase::OnReloading()
{
	// TODO: Play reload animation on client
}

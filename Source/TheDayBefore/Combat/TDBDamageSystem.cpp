// Copyright TheDayBefore Project. All Rights Reserved.

#include "Combat/TDBDamageSystem.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"

void UTDBDamageSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeBodyPartMultipliers();
	UE_LOG(LogTemp, Log, TEXT("Damage System Initialized"));
}

void UTDBDamageSystem::Deinitialize()
{
	Super::Deinitialize();
}

void UTDBDamageSystem::InitializeBodyPartMultipliers()
{
	// Head - critical damage
	BodyPartMultipliers.Add(FName("Head"), 4.0f);
	BodyPartMultipliers.Add(FName("head"), 4.0f);
	
	// Thorax - high damage
	BodyPartMultipliers.Add(FName("Spine1"), 2.0f);
	BodyPartMultipliers.Add(FName("Spine2"), 2.0f);
	BodyPartMultipliers.Add(FName("spine_01"), 2.0f);
	BodyPartMultipliers.Add(FName("spine_02"), 2.0f);
	
	// Arms - medium damage
	BodyPartMultipliers.Add(FName("UpperArm_L"), 1.0f);
	BodyPartMultipliers.Add(FName("UpperArm_R"), 1.0f);
	BodyPartMultipliers.Add(FName("LowerArm_L"), 0.8f);
	BodyPartMultipliers.Add(FName("LowerArm_R"), 0.8f);
	
	// Legs - lower damage
	BodyPartMultipliers.Add(FName("Thigh_L"), 1.0f);
	BodyPartMultipliers.Add(FName("Thigh_R"), 1.0f);
	BodyPartMultipliers.Add(FName("Calf_L"), 0.7f);
	BodyPartMultipliers.Add(FName("Calf_R"), 0.7f);
}

float UTDBDamageSystem::ApplyDamage(AActor* DamagedActor, float BaseDamage, AController* InstigatedBy, FVector HitLocation, FHitResult HitInfo)
{
	if (!DamagedActor || !DamagedActor->CanBeDamaged())
	{
		return 0.0f;
	}

	// Calculate final damage
	float FinalDamage = BaseDamage;

	// Apply armor penetration if we have hit info
	if (HitInfo.BoneName != NAME_None)
	{
		FinalDamage = GetBodyPartDamageMultiplier(HitInfo.BoneName) * BaseDamage;
	}

	// Apply damage through UE's damage system
	float ActualDamage = UGameplayStatics::ApplyDamage(
		DamagedActor,
		FinalDamage,
		InstigatedBy,
		nullptr, // Damage Causer (weapon)
		UDamageType::StaticClass()
	);

	return ActualDamage;
}

float UTDBDamageSystem::CalculateArmorPenetration(float BulletPenetration, float ArmorValue)
{
	// Simple penetration formula
	// Returns value between 0.0 (fully stopped) and 1.0 (fully penetrated)
	if (ArmorValue <= 0.0f)
	{
		return 1.0f; // No armor
	}

	float PenetrationRatio = BulletPenetration / ArmorValue;
	
	// Clamp between 0 and 1
	return FMath::Clamp(PenetrationRatio, 0.0f, 1.0f);
}

float UTDBDamageSystem::CalculateDamageWithArmor(float BaseDamage, float Penetration, float ArmorValue)
{
	if (ArmorValue <= 0.0f)
	{
		return BaseDamage; // No armor
	}

	float PenetrationFactor = CalculateArmorPenetration(Penetration, ArmorValue);
	
	// Damage reduction based on penetration
	// If penetration < armor, damage is significantly reduced
	// If penetration >= armor, damage passes through mostly intact
	float DamageMultiplier = FMath::Lerp(0.1f, 1.0f, PenetrationFactor);
	
	return BaseDamage * DamageMultiplier;
}

void UTDBDamageSystem::ApplyLocalizedDamage(AActor* DamagedActor, float BaseDamage, FName BoneName, AController* InstigatedBy, FHitResult HitInfo)
{
	if (!DamagedActor || !DamagedActor->CanBeDamaged())
	{
		return;
	}

	float Multiplier = GetBodyPartDamageMultiplier(BoneName);
	float FinalDamage = BaseDamage * Multiplier;

	// Apply damage
	UGameplayStatics::ApplyDamage(
		DamagedActor,
		FinalDamage,
		InstigatedBy,
		nullptr,
		UDamageType::StaticClass()
	);

	// TODO: Apply localized effects (bleeding, fractures, etc.)
	UE_LOG(LogTemp, Log, TEXT("Applied %f damage to bone %s (multiplier: %f)"), FinalDamage, *BoneName.ToString(), Multiplier);
}

float UTDBDamageSystem::GetBodyPartDamageMultiplier(FName BoneName)
{
	// Check exact match first
	if (BodyPartMultipliers.Contains(BoneName))
	{
		return BodyPartMultipliers[BoneName];
	}

	// Check if bone name contains key body part names
	FString BoneNameStr = BoneName.ToString();
	
	if (BoneNameStr.Contains("head", ESearchCase::IgnoreCase) || 
	    BoneNameStr.Contains("skull", ESearchCase::IgnoreCase))
	{
		return 4.0f;
	}
	
	if (BoneNameStr.Contains("spine", ESearchCase::IgnoreCase) ||
	    BoneNameStr.Contains("chest", ESearchCase::IgnoreCase))
	{
		return 2.0f;
	}
	
	if (BoneNameStr.Contains("arm", ESearchCase::IgnoreCase) ||
	    BoneNameStr.Contains("hand", ESearchCase::IgnoreCase))
	{
		return 0.8f;
	}
	
	if (BoneNameStr.Contains("leg", ESearchCase::IgnoreCase) ||
	    BoneNameStr.Contains("thigh", ESearchCase::IgnoreCase) ||
	    BoneNameStr.Contains("calf", ESearchCase::IgnoreCase) ||
	    BoneNameStr.Contains("foot", ESearchCase::IgnoreCase))
	{
		return 0.7f;
	}

	// Default multiplier
	return 1.0f;
}

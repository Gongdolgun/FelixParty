#include "Components/WeaponComponent.h"
#include "Global.h"
#include "Weapon/Weapon.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());

	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (TSubclassOf<AWeapon> weaponClass : WeaponClass)
	{
		if (weaponClass)
		{
			AWeapon* weapon = Owner->GetWorld()->SpawnActor<AWeapon>(weaponClass, params);
			Weapons.Add(weapon);
		}
	}
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

AWeapon* UWeaponComponent::GetCurrWeapon()
{
	return Weapons[(int32)Type];
}

void UWeaponComponent::Begin_Equip()
{
	GetCurrWeapon()->Begin_Equip();
}

void UWeaponComponent::End_Equip()
{

}

void UWeaponComponent::Begin_Fire()
{
	if (Weapon)
	{
		Weapon->Fire();
	}
}

void UWeaponComponent::End_Fire()
{

}

void UWeaponComponent::SetPistolMode()
{
	SetMode(EWeaponType::Pistol);
}

void UWeaponComponent::SetRifleMode()
{
	SetMode(EWeaponType::Rifle);
}

void UWeaponComponent::SetShotgunMode()
{
	SetMode(EWeaponType::Shotgun);
}

void UWeaponComponent::SetSniperMode()
{
	SetMode(EWeaponType::Sniper);
}

void UWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
		return;

	else
	{
		ChangeType(InType);
	}
}

void UWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType type = Type;
	Type = InType;

	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(type, InType);
}


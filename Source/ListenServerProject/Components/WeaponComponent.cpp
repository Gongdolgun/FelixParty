#include "Components/WeaponComponent.h"
#include "Global.h"
#include "Weapon/Weapon.h"
#include "Characters/DefaultCharacter.h"

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

void UWeaponComponent::Begin_Equip(int num)
{
	CLog::Print(num);

	/*if (Weapons.IsValidIndex(num))
	{
		Weapon = Weapons[num];

		if (Weapon)
		{
			Weapon->Equip();
		}
	}*/

	/*Weapon = Weapons[num];

	if (Weapon)
	{
		Weapon->Equip();
	}*/

	//if (Weapon)
	//{
	//	Weapon->Equip_Implementation();
	//}
}

void UWeaponComponent::End_Equip()
{

}

void UWeaponComponent::EquipWeapon_1()
{
	Begin_Equip(0);
}

void UWeaponComponent::Begin_Fire()
{
	if (Weapon)
	{
		Weapon->Fire();
	}

	//if (Weapon)
	//{
	//	Weapon->Fire_Implementation();
	//}
}

void UWeaponComponent::End_Fire()
{

}

void UWeaponComponent::SetMode(WeaponType InType)
{
	if (Type == InType)
		return;

	else
	{
		ChangeType(InType);
	}
}

void UWeaponComponent::SetCurrWeapon(AWeapon* NewWeapon)
{
	Weapon = NewWeapon;
}

void UWeaponComponent::ChangeType(WeaponType InType)
{
	WeaponType type = Type;
	Type = InType;

	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(type, InType);
}


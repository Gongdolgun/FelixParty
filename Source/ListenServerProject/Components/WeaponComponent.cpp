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

void UWeaponComponent::Begin_Equip(int32 WeaponIndex)
{
	/*if (Weapons.IsValidIndex(num))
	{
		Weapon = Weapons[num];

		if (Weapon)
		{
			Weapon->Equip();
		}
	}*/

	Weapon = Weapons[WeaponIndex];

	if (Weapon)
	{
		Weapon->Equip();
	}
}

void UWeaponComponent::End_Equip()
{

}

void UWeaponComponent::SelectWeapon(int32 WeaponIndex)
{
	if (WeaponIndex >= 0 && WeaponIndex < Weapons.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Begin Equip: %f"), WeaponIndex);
		Begin_Equip(WeaponIndex);

	}

	//Begin_Equip(WeaponIndex);

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


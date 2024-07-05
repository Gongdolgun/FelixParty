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

void UWeaponComponent::SetGunMode()
{
	SetMode(EWeaponType::Gun);

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


void UWeaponComponent::SetMode(EWeaponType InType)
{
	// Weapon Type이 들어오면
	if (Weapons[(int32)InType] != nullptr)
	{
		// 무기 타입 변경
		Weapons[(int32)InType]->Equip();
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

void UWeaponComponent::SetCurrentWeapon(AWeapon* NewWeapon)
{
	Weapon = NewWeapon;
}

AWeapon* UWeaponComponent::GetCurrentWeapon()
{
	if(IsUnarmedMode())
		return nullptr;

	return Weapons[(int32)Type];
}

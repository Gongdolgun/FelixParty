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

void UWeaponComponent::Begin_Equip()
{
	if (GetCurrentWeapon())
	{
		// 최근 장착한 무기에서 Equip 시작
		GetCurrentWeapon()->Begin_Equip();
	}

	//Weapon = Weapons[num];

	//if (Weapon)
	//{
	//	Weapon->Equip();
	//}

	//if (Weapon)
	//{
	//	Weapon->Equip_Implementation();
	//}
}

void UWeaponComponent::End_Equip()
{

}

void UWeaponComponent::SetUnarmedMode()
{
	// TODO :: UnarmedMode가 되기 위한 조건을 추가해ㅑㅇ함

	GetCurrentWeapon()->UnEquip();
	ChangeType(EWeaponType::Max);
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

	//if (Weapon)
	//{
	//	Weapon->Fire_Implementation();
	//}
}

void UWeaponComponent::End_Fire()
{

}


void UWeaponComponent::SetMode(EWeaponType InType)
{
	// 같은 무기를 누르면 UnArmedMode로 변경
	if (Type == InType)
	{
		SetUnarmedMode();

		return;
	}

	else if (IsUnarmedMode() == false)
	{
		// 무기를 들고 있을 때, 일단 UnEquip으로 모드 변경
		GetCurrentWeapon()->UnEquip();
	}

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

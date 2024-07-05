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
		// �ֱ� ������ ���⿡�� Equip ����
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
	// TODO :: UnarmedMode�� �Ǳ� ���� ������ �߰��ؤ�����

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
	// ���� ���⸦ ������ UnArmedMode�� ����
	if (Type == InType)
	{
		SetUnarmedMode();

		return;
	}

	else if (IsUnarmedMode() == false)
	{
		// ���⸦ ��� ���� ��, �ϴ� UnEquip���� ��� ����
		GetCurrentWeapon()->UnEquip();
	}

	// Weapon Type�� ������
	if (Weapons[(int32)InType] != nullptr)
	{
		// ���� Ÿ�� ����
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

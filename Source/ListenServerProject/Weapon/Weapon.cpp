#include "Weapon/Weapon.h"
#include "Global.h"
#include "Components/WeaponComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Helpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());

}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Attack()
{

}

void AWeapon::Equip()
{
<<<<<<< HEAD
	/*if (Equip_Montage)
		Owner->PlayAnimMontage(Equip_Montage, Montage_PlayRate);*/
=======
	if (Equip_Montage)
		Owner->PlayAnimMontage(Equip_Montage, Montage_PlayRate);

>>>>>>> test

	UE_LOG(LogTemp, Warning, TEXT("Equip_Montage ����: %s"), *Equip_Montage->GetName());
}

void AWeapon::Fire()
{
}


void AWeapon::Begin_Equip()
{
	// ���� ���� ó���� �Ⱥ��̰� �����ؾ���
	if (HolsterSocketName.IsValid())
		Helpers::AttachTo(this, Owner->GetMesh(), HolsterSocketName);
}

void AWeapon::End_Equip()
{

}

void AWeapon::UnEquip()
{
	// TODO :: �������� -> ���� ������� �����
}


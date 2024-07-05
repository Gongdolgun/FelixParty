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
	if (Equip_Montage)
		Owner->PlayAnimMontage(Equip_Montage, Montage_PlayRate);


}

void AWeapon::Fire()
{
}


void AWeapon::Begin_Equip()
{
	// 무기 장착 처음에 안보이게 변경해야함
	if (HolsterSocketName.IsValid())
		Helpers::AttachTo(this, Owner->GetMesh(), HolsterSocketName);
}

void AWeapon::End_Equip()
{

}

void AWeapon::UnEquip()
{
	// TODO :: 장착해제 -> 무기 사라지게 만들기
}


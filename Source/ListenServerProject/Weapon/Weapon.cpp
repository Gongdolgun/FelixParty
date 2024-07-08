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

	// 초기 무기 Attach
	if (AttachSocketName.IsValid())
		Helpers::AttachTo(this, Owner->GetMesh(), AttachSocketName);

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

}

void AWeapon::Fire()
{
}


void AWeapon::Begin_Equip()
{


}

void AWeapon::End_Equip()
{

}

void AWeapon::UnEquip()
{
	// TODO :: 장착해제 -> 무기 사라지게 만들기
}


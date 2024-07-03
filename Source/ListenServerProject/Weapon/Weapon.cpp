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

	if (HolsterSocketName.IsValid())
		Helpers::AttachTo(this, Owner->GetMesh(), HolsterSocketName);

}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Attack()
{

}

void AWeapon::Fire_Implementation()
{

}

void AWeapon::Equip()
{
	if (EquipMontage)
		Owner->PlayAnimMontage(EquipMontage, Montage_PlayRate);
}

void AWeapon::Begin_Equip()
{

}

void AWeapon::End_Equip()
{

}


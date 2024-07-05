#include "Weapon/Guns/Weapon_G_Pistol.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utilites/Helpers.h"

AWeapon_G_Pistol::AWeapon_G_Pistol()
{
}

void AWeapon_G_Pistol::Begin_Equip()
{
	Super::Begin_Equip();
}

void AWeapon_G_Pistol::End_Equip()
{
	Super::End_Equip();
}

void AWeapon_G_Pistol::Equip()
{
	Super::Equip();

	if (Equip_Montage)
		Owner->PlayAnimMontage(Equip_Montage, Montage_PlayRate);
}

void AWeapon_G_Pistol::Fire()
{
	Super::Fire();
}

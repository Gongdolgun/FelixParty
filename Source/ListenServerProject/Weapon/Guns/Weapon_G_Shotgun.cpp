#include "Weapon/Guns/Weapon_G_Shotgun.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utilites/Helpers.h"

AWeapon_G_Shotgun::AWeapon_G_Shotgun()
{
}

void AWeapon_G_Shotgun::Begin_Equip()
{
	Super::Begin_Equip();
}

void AWeapon_G_Shotgun::End_Equip()
{
	Super::End_Equip();
}

void AWeapon_G_Shotgun::Equip()
{
	Super::Equip();

	if (Equip_Montage)
		Owner->PlayAnimMontage(Equip_Montage, Montage_PlayRate);
}

void AWeapon_G_Shotgun::Fire()
{
	Super::Fire();
}

#include "Weapon/Guns/Weapon_G_Rifle.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utilites/Helpers.h"

AWeapon_G_Rifle::AWeapon_G_Rifle()
{

}

void AWeapon_G_Rifle::Begin_Equip()
{
	Super::Begin_Equip();
}

void AWeapon_G_Rifle::End_Equip()
{
	Super::End_Equip();
}

void AWeapon_G_Rifle::Equip()
{
	Super::Equip();

}

void AWeapon_G_Rifle::Fire()
{
	Super::Fire();

	UCameraComponent* camera = Helpers::GetComponent<UCameraComponent>(Owner);
	FVector direction = camera->GetForwardVector();
	FTransform transform = camera->GetComponentToWorld();

	FVector start = transform.GetLocation() + direction;
	FVector end = transform.GetLocation() + direction * HitDistance;

	DrawDebugLine(GetWorld(), start, end, FColor::Red, true, 5);

	printf("Fire On");

}

